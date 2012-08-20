
#ifndef __LOWLEVEL_IO_H_
#define __LOWLEVEL_IO_H_

#include "usb_io.h"

class LowlevelIo: public UsbIo
{
public:
    LowlevelIo();
    virtual ~LowlevelIo();

    int putArgs( int size, unsigned char * data );
    int putString( char * stri );
    int putUInt8( unsigned char val );
    int putUInt16( unsigned short val );
    int putUInt32( unsigned long val );
    bool execFunc( int index );

    bool setParam( int paramId, unsigned char * args, int sz );
    bool param( int paramId, unsigned char * args, int sz );
    bool setEepromParam( int paramId, unsigned char * args, int sz );
    bool eepromParam( int paramId, unsigned char * args, int sz );
};





#endif


