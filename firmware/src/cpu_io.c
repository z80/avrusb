
#include "cpu_io.h"
#include "funcs.h"

#define BUFFER_SZ 32

// Bytes expected
uchar g_ioExpected = 0;
// Buffer pointer
uchar g_ioWrPtr = 0;
uchar g_ioRdPtr = 0;
// Buffer itself
uchar g_ioBuffer[ BUFFER_SZ ];

void cpuIoInit( void )
{
}

void cpuIoReset( void )
{
    g_ioExpected = 0;
    g_ioWrPtr = 0;
    g_ioRdPtr = 0;
}

void cpuIoPush( uchar * in, uchar cnt )
{
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


