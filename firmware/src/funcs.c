
#include "funcs.h"
#include "config.h"

// For debug purpose only
#define MEM_SZ 128
#define EEP_SZ 128
uchar g_ramParams[ MEM_SZ ];
uchar g_eepromParams[ EEP_SZ ];
// / For debug purpose only

static void version( uchar * io );
static void firmware( uchar * io );
static void setParam( uchar * io );
static void param( uchar * io );
static void setEepromParam( uchar * io );
static void eepromParam( uchar * io );

void invoke( uchar id, uchar * io )
{
    switch ( id )
    {
    case FUNC_SET_PARAM:
        setParam( io );
        break;
    case FUNC_PARAM:
        param( io );
        break;
    case FUNC_SET_EEPROM_PARAM:
        setEepromParam( io );
        break;
    case FUNC_EEPROM_PARAM:
        eepromParam( io );
        break;
    case FUNC_FIRMWARE:
        firmware( io );
        break;
    case FUNC_VERSION:
    default:
        version( io );
        break;
    }
}

static void version( uchar * io )
{
    const uchar maxLen = 16;
    uchar ver[] = VERSION;
    uchar i = 0;
    do {
        io[i] = ver[i];
    } while ( ( ver[i] != '\0' ) && ( i < maxLen ) );
}

static void firmware( uchar * io )
{
    const uchar maxLen = 16;
    uchar ver[] = FIRMWARE;
    uchar i = 0;
    do {
        io[i] = ver[i];
    } while ( ( ver[i] != '\0' ) && ( i < maxLen ) );
}

static void setParam( uchar * io )
{
    uchar at = io[0];
    uchar sz = io[1];
    uchar i;
    for ( i=0; i<sz; i++ )
        g_ramParams[ at+i ] = io[i+2];
}

static void param( uchar * io )
{
    uchar at = io[0];
    uchar sz = io[1];
    uchar i;
    for ( i=0; i<sz; i++ )
        io[i+2] = g_ramParams[ at+i ];
}

static void setEepromParam( uchar * io )
{
    uchar at = io[0];
    uchar sz = io[1];
    uchar i;
    for ( i=0; i<sz; i++ )
        g_eepromParams[ at+i ] = io[i+2];
}

static void eepromParam( uchar * io )
{
    uchar at = io[0];
    uchar sz = io[1];
    uchar i;
    for ( i=0; i<sz; i++ )
        io[i+2] = g_eepromParams[ at+i ];
}



