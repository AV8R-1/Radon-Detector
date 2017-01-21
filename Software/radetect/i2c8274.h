#ifndef I2C8274_H
#define I2C8274_H

#include <QObject>

// Class to talk to pcf8274 expander via I2C
// Assumptions:
// Board is jumpered for address 0x21 (default is 0x20)
// I2c is enabled via raspi-config

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

enum LEDStates
{
    LED_OFF,
    LED_ON,
    LED_BLINK
};

struct LED
{
    enum LEDStates state;
    unsigned int bitmask;
};

class I2c8274 : public QObject
{
    Q_OBJECT
public:
    I2c8274( unsigned int address = 0x21 );

public slots:
    void SetLED( int index, unsigned int bitmask, enum LEDStates initState );
    void SetLEDState( int index, enum LEDStates newState );

    void SetLevelRange( int startIndex, int endIndex );
    void SetLevel( int level );
    bool update();

    void Stop() { writeBits(0xff); }

protected:
    unsigned int m_address;
    struct LED m_led[8];
    int m_fh;
    unsigned char m_lastBits;
    int m_levelRange;
    int m_levelBase;

protected:
    bool writeBits(unsigned char b);
};

#endif // I2C8274_H
