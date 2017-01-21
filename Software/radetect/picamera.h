#ifndef PICAMERA_H
#define PICAMERA_H

/***
 * Raspberry Pi camera capture class
 * kush.groover@gmail.com
 * Copyright (C) 2016-2017 Kush Groover. All rights reserved.
 * GPL2 license - you are free to copy and redistribute as long
 * as you don't remove this notice.
 * Partly derived from https://gist.github.com/maxlapshin/1253534
***/

#include <QObject>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#ifndef V4L2_PIX_FMT_H264
#define V4L2_PIX_FMT_H264     v4l2_fourcc('H', '2', '6', '4') /* H264 with start codes */
#endif

enum io_method {
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR,
};

enum init_state {
    INIT_START = 0,
    INIT_OPEN_SUCCESS = 1,
    INIT_OPEN_FAILED = -1,
    INIT_DEVINIT_SUCCESS = 2,
    INIT_DEVINIT_FAILED = -2,
    INIT_STARTCAP_SUCCESS = 3,
    INIT_STARTCAP_FAILED = -3,
    INIT_SHUTDOWN = 4,
    INIT_END
};

struct buffer {
        void   *start;
        size_t  length;
};

class PiCamera : public QObject
{
    Q_OBJECT
public:
    explicit PiCamera(QObject *parent = 0);
    ~PiCamera();

    static void errno_exit(const char *s);
    static bool errno_return(const char *s);
    static const char * FourCCToStr( int fourCC, char *s );
protected:
    bool open_device(void);
    void close_device(void);
    bool init_device(void);
    void uninit_device(void);
    bool start_capturing(void);
    void stop_capturing(void);
    void process_image(const void *p, int size);
    int read_frame(void);
    bool init_userp(unsigned int buffer_size);
    bool init_mmap(void);
    bool init_read(unsigned int buffer_size);

protected:
    char            *dev_name;
    enum io_method   io /* = IO_METHOD_MMAP */;
    int              fd /* = -1*/;
    struct buffer          *buffers;
    unsigned int     n_buffers;
    int              out_buf;
    int              force_format;
    unsigned int    m_v4l2Code;
    int              frame_number /*= 0*/;
    int              frame_count /*= 200*/;
    int             m_missedFrames; // Frames missed due to reentrancy (slow processing of previous frame)
    int             m_width;
    int             m_height;

    int             m_busy; // Set non-zero while processing frame
    enum init_state m_initState; // Initialization state
    bool            m_dbgSaveFrameData; // Write each frame's raw data to disk
signals:
    void Done();
    void FatalError();
    void Alarm(int level);

public slots:
    void SetDevName( const char *devPath );
    void SetSaveFrames( bool dbgSave ) { m_dbgSaveFrameData = dbgSave; }
    void SetSaveFramesOff() { m_dbgSaveFrameData = false; }
    void SetFormat( unsigned int v4l2_code );
    void ProcessFrame();
    void Stop();

};

#endif // PICAMERA_H
