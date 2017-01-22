#include "i2c8274.h"

#include <stdio.h>

I2c8274::I2c8274( unsigned int address )
{
    m_levelBase = -1;
    m_levelRange = -1;
    int n;
    for (n = 0; n < 8; n++)
    {
        m_led[n].bitmask = 0x01 << n;
        m_led[n].state = LED_OFF;
    }
    if ((m_fh = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        printf("Failed to open the i2c bus\n");
        return;
    }

    m_address = address;
    if (ioctl(m_fh, I2C_SLAVE, m_address) < 0)
    {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        //ERROR HANDLING; you can check errno to see what went wrong
        return;
    }

    // Set all LEDs off
    m_lastBits = 0;
    if (writeBits(0xff))
    {
        printf( "Expander init succeeded\n");
    }
    else
    {
        printf( "Initial write failed\n" );
    }
}

void I2c8274::SetLED( int index, unsigned int bitmask, enum LEDStates initState )
{
    if (m_fh == -1) return;
    if (index < 0 || index >= 8) return;
    m_led[index].bitmask = bitmask;
    m_led[index].state = initState;
}

void I2c8274::SetLEDState( int index, enum LEDStates newState )
{
    if (m_fh == -1) return;
    if (index < 0 || index >= 8) return;
    m_led[index].state = newState;
    // Caller will call update when done
}

bool I2c8274::update()
{
    unsigned char bitmask = 0xff; // all off
    int n;
    for (n = 0; n < 8; n++)
    {
        if (m_led[n].state == LED_ON || (m_led[n].state == LED_BLINK /* && it's time to blink */))
        {
            bitmask &= ~m_led[n].bitmask;
        }
        else
        {
            bitmask |= m_led[n].bitmask;
        }
    }
    if (bitmask == m_lastBits)
    {
        return true;
    }
    return writeBits( bitmask );
}

bool I2c8274::writeBits(unsigned char b)
{
    if (write( m_fh, &b, 1 ) != 1)
    {
        return false;
    }
    m_lastBits = b;
    return true;
}

void I2c8274::SetLevelRange(int startIndex, int endIndex)
{
    m_levelBase = startIndex;
    m_levelRange = endIndex - startIndex + 1;
}

void I2c8274::SetLevel(int level)
{
    int n;
    for (n = m_levelBase; n < m_levelBase + m_levelRange; n++)
    {
        if (n < m_levelBase + level) SetLEDState(n, LED_ON);
        else
            SetLEDState(n, LED_OFF);
    }
    update();
}
