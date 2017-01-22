#include <QCoreApplication>
#include <QStringList>
#include <QTimer>

#include <signal.h>

#include "picamera.h"
#include "i2c8274.h"

static PiCamera *g_c = NULL;

void signal_handler( int arg )
{
    switch (arg)
    {
        case SIGINT:
        case SIGTERM:
            QTimer::singleShot(0, g_c, SIGNAL(Stop()));
        break;
    case SIGUSR1:
        printf( "Got SIGUSR1\n" );
        ::fflush(stdout);
        if (g_c) g_c->SetSaveFrames(true);
        break;
    case SIGUSR2:
        printf( "Got SIGUSR2\n" );
        ::fflush(stdout);
        if (g_c) g_c->SetSaveFrames(false);
        break;
    case SIGHUP:
        printf( "Got SIGHUP, saving 4-5 frames\n" );
        ::fflush(stdout);
        if (g_c)
        {
            g_c->SetSaveFrames(true);
            QTimer::singleShot(5 * 1000 / 30, g_c, SLOT(SetSaveFramesOff()));
        }
        break;
    default:
        printf( "Unhandled signal %d\n", arg );
        ::fflush(stdout);
        break;
    }
}

/****
 *
 * Test case:
 * Boot up
 * install http://www.meinbergglobal.com/english/sw/ntp.htm#ntp_stable
 * Edit ntp config on windows and add
 * restrict 192.168.0.20
 * Configure windows firewall and add app ntpd.exe for all networks
 * On pi: sudo sntp -s 192.168.0.12
 * sudo modprobe bcm2835-v4l2
 * /dev/video0 exists now
 ****/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PiCamera c;
    QTimer frameEvent;
    I2c8274 led(0x21);

    QObject::connect( &c, SIGNAL(Done()), &a, SLOT(quit()) );
    QObject::connect( &c, SIGNAL(FatalError()), &a, SLOT(quit()) );
    QObject::connect( &c, SIGNAL(Done()), &led, SLOT(Stop()) );
    QObject::connect( &c, SIGNAL(FatalError()), &led, SLOT(Stop()) );

    led.SetLevelRange( 1, 7 ); // Valid levels are 0-6
    QObject::connect( &c, SIGNAL(Alarm(int)), &led, SLOT(SetLevel(int)) );

    g_c = &c;
    // Default ctrl+c
    signal( SIGINT, signal_handler );
    // Trap normal termination
    signal( SIGTERM, signal_handler );
    // Turn on save frames (killall -s USR1 radetect)
    signal( SIGUSR1, signal_handler );
    // Turn off save frames (killall -s USR2 radetect)
    signal( SIGUSR2, signal_handler );
    // Save next 5 frames
    signal( SIGHUP, signal_handler );

    if (a.arguments().count() > 1)
    {
        c.SetDevName(a.arguments().at(1).toLocal8Bit().constData());
    }
    else
    {
        c.SetDevName("/dev/video0");
    }

    led.SetLED(0, 0x01, LED_ON);
    led.SetLED(1, 0x02, LED_OFF);
    led.SetLED(2, 0x04, LED_OFF);
    led.SetLED(3, 0x08, LED_OFF);
    led.SetLED(4, 0x10, LED_OFF);
    led.SetLED(5, 0x20, LED_OFF);
    led.SetLED(6, 0x40, LED_OFF);
    led.SetLED(7, 0x80, LED_OFF);
    led.update();

    // Raspberry Pi camera module uses YUV420P
    // See https://en.wikipedia.org/wiki/YUV#Y.27UV420p_.28and_Y.27V12_or_YV12.29_to_RGB888_conversion
    // Particularly see the section under https://en.wikipedia.org/wiki/YUV#Y.E2.80.B2UV420p_.28and_Y.E2.80.B2V12_or_YV12.29_to_RGB888_conversion
    // 1.5 bytes per pixel
    // Note that we are only interested in the Y (luma) and can ignore the chroma UV components
    c.SetFormat(V4L2_PIX_FMT_YUV420M);

    // PiCamera object c's state is initially quiescent
    // Start initialization from timer
    QObject::connect( &frameEvent, SIGNAL(timeout()), &c, SLOT(ProcessFrame()) );
    // Fire at frame rate of 30 frames per second
    int fps = 30;
    frameEvent.setInterval( 1000 / fps );
    printf( "Starting picamera v%s at %d fps\n", PiCamera::VERSION.toLocal8Bit().constData(), fps );
    ::fflush(stdout);
    frameEvent.start();

    return a.exec();
}

