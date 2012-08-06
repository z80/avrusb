
#include "ctrlboard_io.h"
#include "config.h"


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

bool CtrlboardIo::version( std::string & ver )
{
    bool res = execFunc( FUNC_VERSION );
    if ( !res )
        return false;
    std::basic_string<unsigned char> & from = dataFrom();
    int sz = read( from, 32 );
    if ( sz < 2 )
        return false;
    ver.resize( sz );
    char * d = reinterpret_cast<char *>( const_cast<unsigned char *>( from.data() ) );
    for ( int i=0; i<sz; i++ )
        ver[i] = d[i];
    return true;
}

bool CtrlboardIo::firmware( std::string & fir )
{
    execFunc( FUNC_FIRMWARE );
    std::basic_string<unsigned char> & from = dataFrom();
    int sz = read( from, 8 );
    if ( sz < 2 )
        return false;
    fir.resize( sz );
    char * d = reinterpret_cast<char *>( const_cast<unsigned char *>( from.data() ) );
    for ( int i=0; i<sz; i++ )
        fir[i] = d[i];
    return true;
}

bool CtrlboardIo::setMotorControl( TMotorControl val )
{
    return true;
}

bool CtrlboardIo::motorControl( TMotorControl & val )
{
    return true;
}

bool CtrlboardIo::setThrottleRumpUp( int val )
{
    return true;
}

bool CtrlboardIo::throttleRumpUp( int & val )
{
    return true;
}

bool CtrlboardIo::setThrottleRumpDown( int val )
{
    return true;
}

bool CtrlboardIo::setThrottleRumpDown( int & val )
{
    return true;
}

bool CtrlboardIo::setThrottleRangeLow( int val )
{
    return true;
}

bool CtrlboardIo::throttleRangeLow( int & val )
{
    return true;
}

bool CtrlboardIo::setThrottleRangeHigh( int val )
{
    return true;
}

bool CtrlboardIo::throttleRangeHigh( int & val )
{
    return true;
}

bool CtrlboardIo::setThrottleLockout( bool val )
{
    return true;
}

bool CtrlboardIo::throttleLockout( bool & val )
{
    return true;
}

bool CtrlboardIo::setStallThreshold( int val )
{
    return true;
}

bool CtrlboardIo::stallThreshold( int & val )
{
    return true;
}

bool CtrlboardIo::setThrottleSpeedCtrl( TSpeedCtrl val )
{
    return true;
}

bool CtrlboardIo::throttleSpeedCtrl( TSpeedCtrl & val )
{
    return true;
}

bool CtrlboardIo::setCurrentLimit( int en )
{
    return true;
}

bool CtrlboardIo::throttleSpeedCtrl( int & en )
{
    return true;
}

bool CtrlboardIo::setUndervoltageCtrl( int en )
{
    return true;
}

bool CtrlboardIo::undervoltageCtrl( int & en )
{
    return true;
}

bool CtrlboardIo::setPassword( const std::string & val )
{
    return true;
}

bool CtrlboardIo::password( std::string & val )
{
    return true;
}

bool CtrlboardIo::setEnabled( bool en )
{
    return true;
}

bool CtrlboardIo::enabled( bool & en )
{
    return true;
}

bool CtrlboardIo::setThrottle( int val )
{
    return true;
}

bool CtrlboardIo::throttle( int & val )
{
    return true;
}

bool CtrlboardIo::setSpeed( int val )
{
    return true;
}

bool CtrlboardIo::speed( int & val )
{
    return true;
}

bool CtrlboardIo::setDirection( bool cw )
{
    return true;
}

bool CtrlboardIo::direction( bool & cw )
{
    return true;
}

bool CtrlboardIo::voltage( int & val )
{
    return true;
}

bool CtrlboardIo::enKeySeat_1( int & val )
{
    return true;
}

bool CtrlboardIo::enKeySeat_2( int & val )
{
    return true;
}

bool CtrlboardIo::controllerT( int & val )
{
    return true;
}

bool CtrlboardIo::motorT( int & val )
{
    return true;
}

bool CtrlboardIo::extT_1( int & val )
{
    return true;
}

bool CtrlboardIo::extT_2( int & val )
{
    return true;
}

bool CtrlboardIo::errorCode( int & val )
{
    return true;
}

bool CtrlboardIo::hours( int & val )
{
    return true;
}

bool CtrlboardIo::cycles( int & val )
{
    return true;
}

bool CtrlboardIo::modelRev( int & val )
{
    return true;
}




