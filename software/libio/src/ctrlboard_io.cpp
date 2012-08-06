
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

bool CtrlboardIo::setParam( int paramId, unsigned char * args, int sz )
{
    std::basic_string<unsigned char> & to = dataTo();
    to.resize( sz+3 );
    to[0] = sz+2;
    to[1] = paramId;
    to[2] = sz;
    for ( int i=0; i<sz; i++ )
        to[i+3] = args[i];
    int cnt = write( to );
    if ( (unsigned int)cnt < to.size() )
        return false;
    bool res = execFunc( FUNC_SET_PARAM );
    return res;
}

bool CtrlboardIo::param( int paramId, unsigned char * args, int sz )
{
    std::basic_string<unsigned char> & to = dataTo();
    to.resize( 3 );
    to[0] = 2;
    to[1] = paramId;
    to[2] = sz;
    int cnt = write( to );
    if ( (unsigned int)cnt < to.size() )
        return false;
    bool res = execFunc( FUNC_PARAM );
    if ( !res )
        return false;
    std::basic_string<unsigned char> & from = dataFrom();
    from.resize( sz );
    cnt = read( from, from.size() );
    if ( (unsigned int)cnt < from.size() )
        return false;
    for ( int i=0; i<sz; i++ )
        args[i] = from[i];
    return true;
}

bool CtrlboardIo::setEepromParam( int paramId, unsigned char * args, int sz )
{
    std::basic_string<unsigned char> & to = dataTo();
    to.resize( sz+3 );
    to[0] = sz+2;
    to[1] = paramId;
    to[2] = sz;
    for ( int i=0; i<sz; i++ )
        to[i+3] = args[i];
    int cnt = write( to );
    if ( (unsigned int)cnt < to.size() )
        return false;
    bool res = execFunc( FUNC_SET_EEPROM_PARAM );
    return res;
}

bool CtrlboardIo::eepromParam( int paramId, unsigned char * args, int sz )
{
    std::basic_string<unsigned char> & to = dataTo();
    to.resize( 3 );
    to[0] = 2;
    to[1] = paramId;
    to[2] = sz;
    int cnt = write( to );
    if ( (unsigned int)cnt < to.size() )
        return false;
    bool res = execFunc( FUNC_EEPROM_PARAM );
    if ( !res )
        return false;
    std::basic_string<unsigned char> & from = dataFrom();
    from.resize( sz );
    cnt = read( from, from.size() );
    if ( (unsigned int)cnt < from.size() )
        return false;
    for ( int i=0; i<sz; i++ )
        args[i] = from[i];
    return true;
}


bool CtrlboardIo::version( std::string & ver )
{
    bool res = execFunc( FUNC_VERSION );
    if ( !res )
        return false;
    std::basic_string<unsigned char> & from = dataFrom();
    int sz = read( from, 8 );
    if ( sz < 2 )
        return false;
    ver.resize( sz );
    char * d = reinterpret_cast<char *>( const_cast<unsigned char *>( from.data() ) );
    for ( int i=0; i<sz; i++ )
    {
        ver[i] = d[i];
        if ( d[i] == '\0' )
            break;
    }
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
    {
        fir[i] = d[i];
        if ( d[i] == '\0' )
            break;
    }
    return true;
}

bool CtrlboardIo::setMotorControl( TMotorControl val )
{
    unsigned char arg = static_cast<unsigned char>( val );
    bool res = setParam( MOTOR_CONTROL, &arg, 1 );
    return res;
}

bool CtrlboardIo::motorControl( TMotorControl & val )
{
    unsigned char arg;
    bool res = param( MOTOR_CONTROL, &arg, 1 );
    if ( res )
        val = static_cast<TMotorControl>( arg );
    return res;
}

bool CtrlboardIo::setThrottleRumpUp( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setParam( THROTTLE_RUMP_UP, arg, 2 );
    return res;
}

bool CtrlboardIo::throttleRumpUp( int & val )
{
    unsigned char arg[2];
    bool res = param( THROTTLE_RUMP_UP, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setThrottleRumpDown( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setParam( THROTTLE_RUMP_DOWN, arg, 2 );
    return res;
}

bool CtrlboardIo::setThrottleRumpDown( int & val )
{
    unsigned char arg[2];
    bool res = param( THROTTLE_RUMP_DOWN, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setThrottleRangeLow( int val )
{
    unsigned char arg = static_cast<unsigned char>( val );
    bool res = setParam( THROTTLE_RANGE_LOW, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttleRangeLow( int & val )
{
    unsigned char arg;
    bool res = param( THROTTLE_RANGE_LOW, &arg, 1 );
    if ( res )
        val = static_cast<int>( arg );
    return res;
}

bool CtrlboardIo::setThrottleRangeHigh( int val )
{
    unsigned char arg = static_cast<unsigned char>( val );
    bool res = setParam( THROTTLE_RANGE_HIGH, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttleRangeHigh( int & val )
{
    unsigned char arg;
    bool res = param( THROTTLE_RANGE_HIGH, &arg, 1 );
    if ( res )
        val = static_cast<int>( arg );
    return res;
}

bool CtrlboardIo::setThrottleLockout( bool val )
{
    unsigned char arg = val ? 1 : 0;
    bool res = setParam( THROTTLE_LOCKOUT, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttleLockout( bool & val )
{
    unsigned char arg;
    bool res = param( THROTTLE_RANGE_HIGH, &arg, 1 );
    if ( res )
        val = ( arg > 0 );
    return res;
}

bool CtrlboardIo::setStallThreshold( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setParam( STALL_THRESHOLD, arg, 2 );
    return res;
}

bool CtrlboardIo::stallThreshold( int & val )
{
    unsigned char arg[2];
    bool res = param( STALL_THRESHOLD, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setThrottleSpeedCtrl( TSpeedCtrl val )
{
    unsigned char arg = static_cast<unsigned char>( val );
    bool res = setParam( MOTOR_CONTROL, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttleSpeedCtrl( TSpeedCtrl & val )
{
    unsigned char arg;
    bool res = param( MOTOR_CONTROL, &arg, 1 );
    if ( res )
        val = static_cast<TSpeedCtrl>( arg );
    return res;
}

bool CtrlboardIo::setCurrentLimit( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setParam( CURRENT_LIMIT, arg, 2 );
    return res;
}

bool CtrlboardIo::throttleSpeedCtrl( int & val )
{
    unsigned char arg[2];
    bool res = param( CURRENT_LIMIT, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setUndervoltageCtrl( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setParam( UNDERVOLTAGE_CTRL, arg, 2 );
    return res;
}

bool CtrlboardIo::undervoltageCtrl( int & val )
{
    unsigned char arg[2];
    bool res = param( UNDERVOLTAGE_CTRL, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setPassword( const std::string & val )
{
    const int SZ = 16;
    unsigned char arg[SZ];
    for ( int i=0; i<SZ; i++ )
        arg[i] = ( i<(int)val.size() ) ? val[i] : '\0';
    bool res = setParam( PASSWORD, arg, SZ );
    return true;
}

bool CtrlboardIo::password( std::string & val )
{
    const int SZ = 16;
    unsigned char arg[SZ];
    bool res = param( PASSWORD, arg, SZ );
    if ( !res )
        return false;
    int sz = 0;
    for ( int i=0; i<SZ; i++ )
    {
        if ( arg[i] == '\0' )
            break;
        sz++;
    }
    val.resize( sz );
    for ( int i=0; i<sz; i++ )
        val[i] = arg[i];
    return true;
}

//bool CtrlboardIo::setEnabled( bool en )
//{
//    return true;
//}
//
//bool CtrlboardIo::enabled( bool & en )
//{
//    return true;
//}

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




