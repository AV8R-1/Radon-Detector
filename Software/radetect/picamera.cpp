#include "picamera.h"

PiCamera::PiCamera(QObject *parent) : QObject(parent)
{
    fd = -1;
    io = IO_METHOD_MMAP;
    frame_number = 0;
    frame_count = 200;
    dev_name = NULL;
    m_busy = 0;
    m_initState = INIT_START;
    force_format = 0;
    m_v4l2Code = 0;
    m_dbgSaveFrameData = false;
    m_missedFrames = 0;
}

PiCamera::~PiCamera()
{
    SetDevName( NULL );
}

void PiCamera::Stop()
{
    if (m_initState < INIT_SHUTDOWN)
    {
        printf( "Stop requested\n" );
        m_initState = INIT_SHUTDOWN;
    }
    else
    {
        printf( "Stop: already in state %d\n", static_cast<int>(m_initState) );
    }
}

void PiCamera::SetDevName( const char *devPath )
{
    if (NULL != dev_name)
    {
        free( dev_name );
        dev_name = NULL;
    }
    if (NULL != devPath)
    {
        dev_name = strdup( devPath );
    }
}

void PiCamera::SetFormat(unsigned int v4l2_code)
{
    if (v4l2_code)
    {
        force_format = 1;
        m_v4l2Code = v4l2_code;
    }
    else
    {
        force_format = 0;
    }
}

void PiCamera::errno_exit(const char *s)
{
        fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
        exit(EXIT_FAILURE);
}

bool PiCamera::errno_return(const char *s)
{
    fprintf( stderr, "%s error %d, %s\n", s, errno, strerror(errno) );
    return false;
}

static int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}

void PiCamera::process_image(const void *p, int size)
{
        frame_number++;
        // m_width is width in pixels
        // m_height is height in pixels
        // Get min, max and look for outliers
        const unsigned char *b = (const unsigned char *)p;
        if (size <= m_width * m_height)
        {
            printf( "Short frame: expected %d, got %d\n", m_width * m_height + m_width * m_height / 2, size );
        }
        else
        {
            // YUV420p serendipitously has the first width x height bytes containing luma values
            // That's all we're interested in!
            unsigned char minV = 255;
            unsigned char maxV = 0;
            int maxX = 0, maxY = 0;
            int pixel;
            // Mask off unreliable pixels. These sometimes return false positives
            int badPix[] = { 0*m_width + 67, 303*m_width + 28, 479*m_width + 631 };
            int n;
            for (pixel = 0; pixel < m_width * m_height; pixel++)
            {
                if (b[pixel] < minV) minV = b[pixel];
                if (b[pixel] > maxV)
                {
                    bool ignore = false;
                    for (n = 0; n < sizeof(badPix)/sizeof(badPix[0]); n++)
                    {
                        if (pixel == badPix[n]) ignore = true;
                    }
                    if (ignore) continue;
                    maxV = b[pixel];
                    maxY = pixel / m_width;
                    maxX = pixel % m_width;
                }
            }
            // FIXME make threshold configurable
            // FIXME calculate peak altitude
            if (maxV > 26)
            {
                // FIXME record position of last max
                printf( "Max pixel %u detected at [%d]@%d,%d\n",  maxV, frame_number, maxX, maxY );
                //printf( "(bp = %d,%d, pix=%d)\n", badPix[0], badPix[1], maxY * m_width + maxX );
                emit Alarm( 2 );
            }
        }
        if (m_dbgSaveFrameData)
        {
        char filename[15];
        sprintf(filename, "frame-%d.raw", frame_number);
        FILE *fp=fopen(filename,"wb");

        if (out_buf)
                fwrite(p, size, 1, fp);

        fflush(fp);
        fclose(fp);
        }
}

int PiCamera::read_frame(void)
{
        struct v4l2_buffer buf;
        unsigned int i;

        switch (io) {
        case IO_METHOD_READ:
                if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                                m_initState = INIT_SHUTDOWN;
                                return errno_return("read") ? 1 : 0;
                        }
                }

                process_image(buffers[0].start, buffers[0].length);
                break;

        case IO_METHOD_MMAP:
                CLEAR(buf);

                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;

                if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                                m_initState = INIT_SHUTDOWN;
                                return errno_return("VIDIOC_DQBUF") ? 1 : 0;
                        }
                }

                assert(buf.index < n_buffers);

                process_image(buffers[buf.index].start, buf.bytesused);

                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                {
                    m_initState = INIT_SHUTDOWN;
                    return errno_return("VIDIOC_QBUF") ? 1 : 0;
                }
                break;

        case IO_METHOD_USERPTR:
                CLEAR(buf);

                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_USERPTR;

                if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                            m_initState = INIT_SHUTDOWN;
                            return errno_return("VIDIOC_DQBUF") ? 1 : 0;
                        }
                }

                for (i = 0; i < n_buffers; ++i)
                        if (buf.m.userptr == (unsigned long)buffers[i].start
                            && buf.length == buffers[i].length)
                                break;

                assert(i < n_buffers);

                process_image((void *)buf.m.userptr, buf.bytesused);

                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                {
                    m_initState = INIT_SHUTDOWN;
                        return errno_return("VIDIOC_QBUF") ? 1 : 0;
                }
                break;
        }

        return 1;
}

#if 0
static void mainloop(void)
{
        unsigned int count;

        count = frame_count;

        while (count-- > 0) {
                for (;;) {
                        fd_set fds;
                        struct timeval tv;
                        int r;

                        FD_ZERO(&fds);
                        FD_SET(fd, &fds);

                        /* Timeout. */
                        tv.tv_sec = 2;
                        tv.tv_usec = 0;

                        r = select(fd + 1, &fds, NULL, NULL, &tv);

                        if (-1 == r) {
                                if (EINTR == errno)
                                        continue;
                                errno_exit("select");
                        }

                        if (0 == r) {
                                fprintf(stderr, "select timeout\n");
                                exit(EXIT_FAILURE);
                        }

                        if (read_frame())
                                break;
                        /* EAGAIN - continue select loop. */
                }
        }
}
#endif

void PiCamera::stop_capturing(void)
{
        enum v4l2_buf_type type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
                        errno_exit("VIDIOC_STREAMOFF");
                break;
        }
}

bool PiCamera::start_capturing(void)
{
        unsigned int i;
        enum v4l2_buf_type type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
                for (i = 0; i < n_buffers; ++i) {
                        struct v4l2_buffer buf;

                        CLEAR(buf);
                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        buf.memory = V4L2_MEMORY_MMAP;
                        buf.index = i;

                        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                                return errno_return("VIDIOC_QBUF");
                }
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
                        return errno_return("VIDIOC_STREAMON");
                break;

        case IO_METHOD_USERPTR:
                for (i = 0; i < n_buffers; ++i) {
                        struct v4l2_buffer buf;

                        CLEAR(buf);
                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        buf.memory = V4L2_MEMORY_USERPTR;
                        buf.index = i;
                        buf.m.userptr = (unsigned long)buffers[i].start;
                        buf.length = buffers[i].length;

                        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                                return errno_return("VIDIOC_QBUF");
                }
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
                        return errno_return("VIDIOC_STREAMON");
                break;
        }
        return true;
}

void PiCamera::uninit_device(void)
{
        unsigned int i;

        switch (io) {
        case IO_METHOD_READ:
                free(buffers[0].start);
                break;

        case IO_METHOD_MMAP:
                for (i = 0; i < n_buffers; ++i)
                        if (-1 == munmap(buffers[i].start, buffers[i].length))
                                errno_return("munmap");
                break;

        case IO_METHOD_USERPTR:
                for (i = 0; i < n_buffers; ++i)
                        free(buffers[i].start);
                break;
        }

        free(buffers);
}

bool PiCamera::init_read(unsigned int buffer_size)
{
        buffers = (struct buffer *)calloc(1, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\n");
                return false;
        }

        buffers[0].length = buffer_size;
        buffers[0].start = malloc(buffer_size);

        if (!buffers[0].start) {
                fprintf(stderr, "Out of memory\n");
                return false;
        }
        return true;
}

bool PiCamera::init_mmap(void)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support "
                                 "memory mapping\n", dev_name);
                        return false;
                } else {
                        return errno_return("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2) {
                fprintf(stderr, "Insufficient buffer memory on %s\n",
                         dev_name);
                return false;
        }

        buffers = (struct buffer *)calloc(req.count, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\n");
                return false;
        }

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                struct v4l2_buffer buf;

                CLEAR(buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

                if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
                        return errno_return("VIDIOC_QUERYBUF");

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start =
                        mmap(NULL /* start anywhere */,
                              buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd, buf.m.offset);

                if (MAP_FAILED == buffers[n_buffers].start)
                        return errno_return("mmap");
        }
        return true;
}

bool PiCamera::init_userp(unsigned int buffer_size)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count  = 4;
        req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support "
                                 "user pointer i/o\n", dev_name);
                        return false;
                } else {
                        return errno_return("VIDIOC_REQBUFS");
                }
        }

        buffers = (struct buffer *)calloc(4, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\n");
                return false;
        }

        for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
                buffers[n_buffers].length = buffer_size;
                buffers[n_buffers].start = malloc(buffer_size);

                if (!buffers[n_buffers].start) {
                        fprintf(stderr, "Out of memory\n");
                        return false;
                }
        }
        return true;
}

bool PiCamera::init_device(void)
{
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;
        unsigned int min;

        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s is no V4L2 device\n",
                                 dev_name);
                        return false;
                } else {
                        return errno_return("VIDIOC_QUERYCAP");
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf(stderr, "%s is no video capture device\n",
                         dev_name);
                return false;
        }

        switch (io) {
        case IO_METHOD_READ:
                if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
                        fprintf(stderr, "%s does not support read i/o\n",
                                 dev_name);
                        return false;
                }
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                        fprintf(stderr, "%s does not support streaming i/o\n",
                                 dev_name);
                        return false;
                }
                break;
        }


        /* Select video input, video standard and tune here. */


        CLEAR(cropcap);

        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
                crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                crop.c = cropcap.defrect; /* reset to default */

                if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
                        switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                                break;
                        default:
                                /* Errors ignored. */
                                break;
                        }
                }
        } else {
                /* Errors ignored. */
        }


        CLEAR(fmt);

        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (force_format) {
            char fourCC[64];
    fprintf(stderr, "Set %s\r\n", FourCCToStr(m_v4l2Code, fourCC));
                fmt.fmt.pix.width       = 640; //replace
                fmt.fmt.pix.height      = 480; //replace
                fmt.fmt.pix.pixelformat = m_v4l2Code; //V4L2_PIX_FMT_H264; //replace
                fmt.fmt.pix.field       = V4L2_FIELD_ANY;

                if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
                        return errno_return("VIDIOC_S_FMT");

                /* Note VIDIOC_S_FMT may change width and height. */
        } else {
                /* Preserve original settings as set by v4l2-ctl for example */
                if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
                        return errno_return("VIDIOC_G_FMT");
        }

        /* Buggy driver paranoia. */
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;

        m_width = fmt.fmt.pix.width;
        m_height = fmt.fmt.pix.height;

        char pixfmt_4cc[64];
        printf( "Format: %dX%d (%d bytes per line, %d total, pixfmt=%s)\n", fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.bytesperline, fmt.fmt.pix.sizeimage, FourCCToStr(fmt.fmt.pix.pixelformat, pixfmt_4cc) );

        switch (io) {
        case IO_METHOD_READ:
                return init_read(fmt.fmt.pix.sizeimage);
                break;

        case IO_METHOD_MMAP:
                return init_mmap();
                break;

        case IO_METHOD_USERPTR:
                return init_userp(fmt.fmt.pix.sizeimage);
                break;
        }
        return true;
}

void PiCamera::close_device(void)
{
        if (-1 == close(fd))
                errno_exit("close");

        fd = -1;
}

bool PiCamera::open_device(void)
{
        struct stat st;

        if (-1 == stat(dev_name, &st)) {
                fprintf(stderr, "Cannot identify '%s': %d, %s\n",
                         dev_name, errno, strerror(errno));
                return false;
        }

        if (!S_ISCHR(st.st_mode)) {
                fprintf(stderr, "%s is no device\n", dev_name);
                return false;
        }

        fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd) {
                fprintf(stderr, "Cannot open '%s': %d, %s\n",
                         dev_name, errno, strerror(errno));
                return false;
        }
        return true;
}

/**************
static void usage(FILE *fp, int argc, char **argv)
{
        fprintf(fp,
                 "Usage: %s [options]\n\n"
                 "Version 1.3\n"
                 "Options:\n"
                 "-d | --device name   Video device name [%s]\n"
                 "-h | --help          Print this message\n"
                 "-m | --mmap          Use memory mapped buffers [default]\n"
                 "-r | --read          Use read() calls\n"
                 "-u | --userp         Use application allocated buffers\n"
                 "-o | --output        Outputs stream to stdout\n"
                 "-f | --format        Force format to 640x480 YUYV\n"
                 "-c | --count         Number of frames to grab [%i]\n"
                 "",
                 argv[0], dev_name, frame_count);
}

static const char short_options[] = "d:hmruofc:";

static const struct option
long_options[] = {
        { "device", required_argument, NULL, 'd' },
        { "help",   no_argument,       NULL, 'h' },
        { "mmap",   no_argument,       NULL, 'm' },
        { "read",   no_argument,       NULL, 'r' },
        { "userp",  no_argument,       NULL, 'u' },
        { "output", no_argument,       NULL, 'o' },
        { "format", no_argument,       NULL, 'f' },
        { "count",  required_argument, NULL, 'c' },
        { 0, 0, 0, 0 }
};
****/

#if 0
int main(int argc, char **argv)
{
        dev_name = "/dev/video0";

        for (;;) {
                int idx;
                int c;

                c = getopt_long(argc, argv,
                                short_options, long_options, &idx);

                if (-1 == c)
                        break;

                switch (c) {
                case 0: /* getopt_long() flag */
                        break;

                case 'd':
                        dev_name = optarg;
                        break;

                case 'h':
                        usage(stdout, argc, argv);
                        exit(EXIT_SUCCESS);

                case 'm':
                        io = IO_METHOD_MMAP;
                        break;

                case 'r':
                        io = IO_METHOD_READ;
                        break;

                case 'u':
                        io = IO_METHOD_USERPTR;
                        break;

                case 'o':
                        out_buf++;
                        break;

                case 'f':
                        force_format++;
                        break;

                case 'c':
                        errno = 0;
                        frame_count = strtol(optarg, NULL, 0);
                        if (errno)
                                errno_exit(optarg);
                        break;

                default:
                        usage(stderr, argc, argv);
                        exit(EXIT_FAILURE);
                }
        }

        open_device();
        init_device();
        start_capturing();
        mainloop();
        stop_capturing();
        uninit_device();
        close_device();
        fprintf(stderr, "\n");
        return 0;
}
#endif

// Slot called by timer at 30hz
void PiCamera::ProcessFrame()
{
    // FIXME keep track of missed frames!
    if (m_busy)
    {
        m_missedFrames++;
        return;
    }

    m_busy++;
    switch (m_initState)
    {
    case INIT_START:
        if (!open_device())
        {
            m_initState = INIT_OPEN_FAILED;
            emit FatalError();
            m_busy = 0;
            return;
        }
        m_initState = INIT_OPEN_SUCCESS;
        printf( "Device opened\n" );
        m_busy = 0;
        return;
    case INIT_OPEN_SUCCESS:
        if (!init_device())
        {
            m_initState = INIT_DEVINIT_FAILED;
            m_busy = 0;
            return;
        }
        m_initState = INIT_DEVINIT_SUCCESS;
        printf( "Device initialized\n" );
        m_busy = 0;
        return;
    case INIT_DEVINIT_FAILED:
        close_device();
        m_busy = 0;
        emit FatalError();
        return;
    case INIT_DEVINIT_SUCCESS:
        if (!start_capturing())
        {
            m_initState = INIT_STARTCAP_FAILED;
            m_busy = 0;
            return;
        }
        m_initState = INIT_STARTCAP_SUCCESS;
        printf( "Capture started\n" );
        m_busy = 0;
        return;
    case INIT_STARTCAP_FAILED:
        uninit_device();
        m_initState = INIT_DEVINIT_FAILED;
        m_busy = 0;
        return;
    case INIT_STARTCAP_SUCCESS:
        break;
    case INIT_SHUTDOWN:
        printf( "Shutting down...\n" );
        stop_capturing();
        printf( "Capture stopped, total frames %d (missed %d)\n", frame_number, m_missedFrames );
        uninit_device();
        printf( "Device uninitialized\n" );
        close_device();
        printf( "Device closed\n" );
        m_busy = 0;
        m_initState = INIT_END;
        return;
    case INIT_END:
        m_busy = 0;
        printf( "Exiting...\n" );
        emit Done();
        return;
    }

    // State should be INIT_STARTCAP_SUCCESS

    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    r = select(fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r) {
            if (EINTR == errno)
                    goto frame_exit;
            // Other error
            errno_return("select");
            m_initState = INIT_SHUTDOWN;
            goto frame_exit;
    }

    if (0 == r) {
            fprintf(stderr, "select timeout\n");
            m_initState = INIT_SHUTDOWN;
            goto frame_exit;
    }

    if (read_frame())
    {
        // Analyze frame data in read_image()
    }
    /* EAGAIN - continue select loop. */
    // Count missed frame

frame_exit:
    m_busy = 0;
}

const char * PiCamera::FourCCToStr(int fourCC, char *s)
{
    s[0] = (fourCC & 0xff000000) >> 24;
    s[1] = (fourCC & 0x00ff0000) >> 16;
    s[2] = (fourCC & 0x0000ff00) >> 8;
    s[3] = (fourCC & 0x000000ff) >> 0;
    s[4] = '\0';
    return s;
}
