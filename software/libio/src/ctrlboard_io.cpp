
#include "ctrlboard_io.h"

inline static bool LSB()
{
    int i = 1;
    unsigned char * p = reinterpret_cast<unsigned char *>( &i );
    bool res = (p[0] != 0);
    return res;
}

inline static int toUInt16( unsigned char d[] )
{
    int res;
    if ( LSB() )
        res = (static_cast<int>( d[0] ) << 8) + d[1];
    else
        res = (static_cast<int>( d[1] ) << 8) + d[0];
    return res;
}

CtrlboardIo::CtrlboardIo()
: LowlevelIo()
{
}

CtrlboardIo::~CtrlboardIo()
{
}

int  CtrlboardIo::version()
{
    execFunc( FUNC_VERSION );
    unsigned char d[2];
    int sz = 3; //readQueue( d, 2 );
    if ( sz < 2 )
        return -1;
    int res = d[0] + d[1] * 256;
    return res;
}






