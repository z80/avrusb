
#include "cpu_io.h"
#include "funcs.h"
#include "dbg.h"

#define BUFFER_SZ 32

// Bytes expected
uchar g_ioExpected = 0;
// Buffer pointer
uchar g_ioWrPtr = 0;
uchar g_ioRdPtr = 0;
// Buffer itself
uchar g_ioBuffer[ BUFFER_SZ ];

#define IO_WATCHDOG_TOP    200 // 2 seconds if timer0 counts up to 100 with 1024 prescaler at 20MHz.
uint16_t g_ioWatchdog = 0;

void cpuIoInit( void )
{
    TCCR0 = 0; // disable timer0;
    TCNT0 = 0; // reset it's counter;
    TCCR0 = (1 << CS02) | (1 << CS00); // run timer0 with x1024 prescaler.
}

void cpuIoPoll( void )
{
    if ( TCNT0 >= 100 )
    {
        g_ioWatchdog++;
        if ( g_ioWatchdog > IO_WATCHDOG_TOP )
        {
            cpuIoReset();
            blinkLed0();
        }
        cpuIoInit();
    }
}

void cpuIoReset( void )
{
    // IO watchdog reset.
    g_ioWatchdog = 0;

    g_ioExpected = 0;
    g_ioWrPtr = 0;
    g_ioRdPtr = 0;
}

void cpuIoPush( uchar * in, uchar cnt )
{
    // IO watchdog reset.
    g_ioWatchdog = 0;

    uchar i = 0;
    if ( g_ioExpected == 0 )
    {
        g_ioExpected = in[0];
        i++;
    }

    if ( g_ioExpected > 0 )
    {
        // Filling buffer with arguments.
        for ( ;i<cnt; i++ )
        {
            g_ioBuffer[ g_ioWrPtr ] = in[i];
            g_ioWrPtr = (g_ioWrPtr + 1) % BUFFER_SZ;
            g_ioExpected--;
        }
    }
    else
    {
        // Function invocation.
        cpuIoReset();
        invoke( in[1], g_ioBuffer );
    }
}

void cpuIoPop( uchar * out, uchar cnt )
{
    uchar i;
    for ( i=0; i<cnt; i++ )
    {
        out[i] = g_ioBuffer[ g_ioRdPtr ];
        g_ioRdPtr = ( g_ioRdPtr + 1 ) % BUFFER_SZ;
    }
}


