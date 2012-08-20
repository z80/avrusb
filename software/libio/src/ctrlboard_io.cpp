
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
    QMutexLocker lock( &m_mutex );

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
    QMutexLocker lock( &m_mutex );

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

bool CtrlboardIo::setThrottleType( TThrottleType val )
{
    unsigned char arg = static_cast<unsigned char>( val );
    bool res = setEepromParam( THROTTLE_TYPE, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttleType( TThrottleType & val )
{
    unsigned char arg;
    bool res = eepromParam( THROTTLE_TYPE, &arg, 1 );
    if ( res )
        val = static_cast<TThrottleType>( arg );
    return res;
}

bool CtrlboardIo::setThrottleMode( TThrottleMode val )
{
    unsigned char arg = static_cast<unsigned char>( val );
    bool res = setEepromParam( THROTTLE_MODE, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttleMode( TThrottleMode & val )
{
    unsigned char arg;
    bool res = eepromParam( THROTTLE_MODE, &arg, 1 );
    if ( res )
        val = static_cast<TThrottleMode>( arg );
    return res;
}

bool CtrlboardIo::setMaxThrottleCw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( MAX_THROTTLE_CW, arg, 2 );
    return res;
}

bool CtrlboardIo::maxThrottleCw( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( MAX_THROTTLE_CW, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setMaxThrottleCcw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( MAX_THROTTLE_CCW, arg, 2 );
    return res;
}

bool CtrlboardIo::maxThrottleCcw( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( MAX_THROTTLE_CCW, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setMaxSpeedCw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( MAX_SPEED_CW, arg, 2 );
    return res;
}

bool CtrlboardIo::maxSpeedCw( int & val )
{
  unsigned char arg[2];
  bool res = eepromParam( MAX_SPEED_CW, arg, 2 );
  if ( res )
      val = arg[0] + 256 * arg[1];
  return res;
}

bool CtrlboardIo::setMaxSpeedCcw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( MAX_SPEED_CCW, arg, 2 );
    return res;
}

bool CtrlboardIo::maxSpeedCcw( int & val )
{
  unsigned char arg[2];
  bool res = eepromParam( MAX_SPEED_CCW, arg, 2 );
  if ( res )
      val = arg[0] + 256 * arg[1];
  return res;
}

bool CtrlboardIo::setThrottleRampUpCw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( THROTTLE_RAMP_UP_CW, arg, 2 );
    return res;
}

bool CtrlboardIo::throttleRampUpCw( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( THROTTLE_RAMP_UP_CW, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setThrottleRampUpCcw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( THROTTLE_RAMP_UP_CCW, arg, 2 );
    return res;
}

bool CtrlboardIo::throttleRampUpCcw( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( THROTTLE_RAMP_UP_CCW, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setThrottleRampDownCw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( THROTTLE_RAMP_DOWN_CW, arg, 2 );
    return res;
}

bool CtrlboardIo::throttleRampDownCw( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( THROTTLE_RAMP_DOWN_CW, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setThrottleRampDownCcw( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( THROTTLE_RAMP_DOWN_CCW, arg, 2 );
    return res;
}

bool CtrlboardIo::throttleRampDownCcw( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( THROTTLE_RAMP_DOWN_CCW, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setCommutationMode( int val )
{
    unsigned char arg = static_cast<unsigned char>( val );
    bool res =setEepromParam( COMMUTATION_MODE, &arg, 1 );
    return res;
}

bool CtrlboardIo::commutationMode( int & val )
{
    unsigned char arg;
    bool res = eepromParam( COMMUTATION_MODE, &arg, 1 );
    if ( res )
      val = static_cast<int>( arg );
    return res;
}

bool CtrlboardIo::setThrottleLockout( bool val )
{
    unsigned char arg = val ? 1 : 0;
    bool res = setEepromParam( THROTTLE_LOCKOUT, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttleLockout( bool & val )
{
    unsigned char arg;
    bool res = eepromParam( THROTTLE_LOCKOUT, &arg, 1 );
    if ( res )
        val = ( arg > 0 );
    return res;
}

bool CtrlboardIo::setStallThreshold( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( STALL_THRESHOLD, arg, 2 );
    return res;
}

bool CtrlboardIo::stallThreshold( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( STALL_THRESHOLD, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setCurrentLimit( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( CURRENT_LIMIT, arg, 2 );
    return res;
}

bool CtrlboardIo::currentLimit( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( CURRENT_LIMIT, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setUndervoltageCtrl( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( UNDERVOLTAGE_CTRL, arg, 2 );
    return res;
}

bool CtrlboardIo::undervoltageCtrl( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( UNDERVOLTAGE_CTRL, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setMotorOvertemp( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( MOTOR_OVERTEMP, arg, 2 );
    return res;
}

bool CtrlboardIo::motorOvertemp( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( MOTOR_OVERTEMP, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setControllerOvertemp( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setEepromParam( CONTROLLER_OVERTEMP, arg, 2 );
    return res;
}

bool CtrlboardIo::controllerOvertemp( int & val )
{
  unsigned char arg[2];
  bool res = eepromParam( CONTROLLER_OVERTEMP, arg, 2 );
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
    bool res = setEepromParam( PASSWORD, arg, SZ );
    return true;
}

bool CtrlboardIo::password( std::string & val )
{
    const int SZ = 16;
    unsigned char arg[SZ];
    bool res = eepromParam( PASSWORD, arg, SZ );
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
    unsigned char arg = static_cast<unsigned char>( val );
    bool res = setParam( THROTTLE, &arg, 1 );
    return res;
}

bool CtrlboardIo::throttle( int & val )
{
    unsigned char arg;
    bool res = param( THROTTLE, &arg, 1 );
    if ( res )
        val = static_cast<int>( arg );
    return res;
}

bool CtrlboardIo::setSpeed( int val )
{
    unsigned char arg[2];
    arg[0] = val & 255;
    arg[1] = (val >> 8) & 255;
    bool res = setParam( SPEED, arg, 2 );
    return res;
}

bool CtrlboardIo::speed( int & val )
{
    unsigned char arg[2];
    bool res = param( SPEED, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::setDirection( bool cw )
{
    unsigned char arg = cw ? 1 : 0;
    bool res = setParam( DIRECTION, &arg, 1 );
    return res;
}

bool CtrlboardIo::direction( bool & cw )
{
    unsigned char arg;
    bool res = param( DIRECTION, &arg, 1 );
    if ( res )
        cw = ( arg > 0 );
    return res;
}

bool CtrlboardIo::voltage( int & val )
{
    unsigned char arg[2];
    bool res = param( VOLTAGE, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::enKeySeat_1( bool & val )
{
    unsigned char arg;
    bool res = param( ENABLE_KEY_SEAT_1, &arg, 1 );
    if ( res )
        val = ( arg > 0 );
    return res;
}

bool CtrlboardIo::enKeySeat_2( bool & val )
{
    unsigned char arg;
    bool res = param( ENABLE_KEY_SEAT_2, &arg, 1 );
    if ( res )
        val = ( arg > 0 );
    return res;
}

bool CtrlboardIo::controllerT( int & val )
{
    unsigned char arg[2];
    bool res = param( CONTROLLER_T, arg, 2 );
    if ( res )
    {
        val = arg[0] + 256 * arg[1];
        if ( val & 32768 )
        	val -= 65535;
    }
    return res;
}

bool CtrlboardIo::motorT( int & val )
{
    unsigned char arg[2];
    bool res = param( MOTOR_T, arg, 2 );
    if ( res )
    {
        val = arg[0] + 256 * arg[1];
        if ( val & 32768 )
        	val -= 65535;
    }
    return res;
}

bool CtrlboardIo::extT_1( int & val )
{
    unsigned char arg[2];
    bool res = param( EXT_T_1, arg, 2 );
    if ( res )
    {
        val = arg[0] + 256 * arg[1];
        if ( val & 32768 )
        	val -= 65535;
    }
    return res;
}

bool CtrlboardIo::extT_2( int & val )
{
    unsigned char arg[2];
    bool res = param( EXT_T_2, arg, 2 );
    if ( res )
    {
        val = arg[0] + 256 * arg[1];
        if ( val & 32768 )
        	val -= 65535;
    }
    return res;
}

bool CtrlboardIo::errorCode( int & val )
{
    unsigned char arg;
    bool res = param( ERROR, &arg, 1 );
    if ( res )
        val = static_cast<int>( arg );
    return res;
}

bool CtrlboardIo::hours( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( HOURS, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::cycles( int & val )
{
    unsigned char arg[2];
    bool res = eepromParam( CYCLES, arg, 2 );
    if ( res )
        val = arg[0] + 256 * arg[1];
    return res;
}

bool CtrlboardIo::modelRev( std::string & val )
{
    const int SZ = 16;
    unsigned char arg[SZ];
    bool res = eepromParam( MODEL_REV, arg, SZ );
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




