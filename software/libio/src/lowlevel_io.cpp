
#include "lowlevel_io.h"
#include "string.h"
#include "config.h"

const int LowlevelIo::EXECUTION_TIME = 50;

class Sleep: public QThread
{
public:
	static void msleep( int ms = LowlevelIo::EXECUTION_TIME )
	{
		QThread::msleep( ms );
	}
};

inline static bool LSB()
{
    int i = 1;
    unsigned char * p = reinterpret_cast<unsigned char *>( &i );
    bool res = (p[0] != 0);
    return res;
}

LowlevelIo::LowlevelIo()
: UsbIo()
{
}

LowlevelIo::~LowlevelIo()
{
}

int LowlevelIo::putArgs( int size, unsigned char * args )
{
    std::basic_string<unsigned char> & data = this->dataTo();
    for ( int i=0; i<size; i+=(maxPacketSize()-1) )
    {
        int sz = ( (size - i) < (maxPacketSize()-1) ) ? (size - i) : (maxPacketSize()-1);
        data.resize( sz+1 );
        data[0] = sz;
        for ( int j=0; j<data[0]; j++ )
            data[j+1] = args[i+j];
        unsigned char * d = args;
        int res = write( data );
        if ( res < 0 )
            return res;
    }
    return size;
}

int LowlevelIo::putString( char * stri )
{
    int sz = strlen( stri );
    int res = putArgs( sz, reinterpret_cast<unsigned char *>( stri ) );
    return res;
}

int LowlevelIo::putUInt8( unsigned char val )
{
    int res = putArgs( sizeof(unsigned char), &val );
    return res;
}

// Here it is necessary to revert numbers to make things compatible with target's bytes order.
int LowlevelIo::putUInt16( unsigned short val )
{
    unsigned char * p = reinterpret_cast<unsigned char *>( &val );
    if ( LSB() )
    {
        int res = putArgs( sizeof(unsigned short), p );
        return res;
    }
    else
    {
        unsigned char b[ sizeof(unsigned short) ];
        b[0] = p[1];
        b[1] = p[0];
        int res = putArgs( sizeof(unsigned short), b );
        return res;
    }
}

int LowlevelIo::putUInt32( unsigned long val )
{
    unsigned char * p = reinterpret_cast<unsigned char *>( &val );
    if ( LSB() )
    {
        int res = putArgs( sizeof(unsigned long), p );
        return res;
    }
    else
    {
        unsigned char b[ sizeof(unsigned long) ];
        b[0] = p[3];
        b[1] = p[2];
        b[2] = p[1];
        b[3] = p[0];
        int res = putArgs( sizeof(unsigned long), b );
        return res;
    }
}

bool LowlevelIo::execFunc( int index )
{
    std::basic_string<unsigned char> & data = this->dataTo();
    data.clear();
    int sz = 2;
    data.resize( sz );
    data[0] = 0;
    data[1] = static_cast<unsigned char>( index );
    int res = write( data );
    Sleep::msleep();
    return ( res == sz );
}

bool LowlevelIo::setParam( int paramId, unsigned char * args, int sz )
{
    QMutexLocker lock( &m_mutex );

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

bool LowlevelIo::param( int paramId, unsigned char * args, int sz )
{
    QMutexLocker lock( &m_mutex );

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

bool LowlevelIo::setEepromParam( int paramId, unsigned char * args, int sz )
{
    QMutexLocker lock( &m_mutex );

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

bool LowlevelIo::eepromParam( int paramId, unsigned char * args, int sz )
{
    QMutexLocker lock( &m_mutex );

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








