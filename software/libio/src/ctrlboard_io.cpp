
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

int  CtrlboardIo::version( std::string & ver )
{
    execFunc( FUNC_VERSION );
    std::basic_string<unsigned char> & from = dataFrom();
    int sz = read( from, 8 );
    if ( sz < 2 )
        return -1;
    ver.resize( sz );
    char * d = reinterpret_cast<char *>( const_cast<unsigned char *>( from.data() ) );
    for ( int i=0; i<sz; i++ )
        ver[i] = d[i];
    return sz;
}

int  CtrlboardIo::firmware( std::string & fir )
{
    execFunc( FUNC_FIRMWARE );
    std::basic_string<unsigned char> & from = dataFrom();
    int sz = read( from, 8 );
    if ( sz < 2 )
        return -1;
    fir.resize( sz );
    char * d = reinterpret_cast<char *>( const_cast<unsigned char *>( from.data() ) );
    for ( int i=0; i<sz; i++ )
        fir[i] = d[i];
    return sz;
}






