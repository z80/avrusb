
#ifndef __USB_IO_H_
#define __USB_IO_H_

#include <string>

class UsbIo
{
public:
    UsbIo();
    virtual ~UsbIo();
    bool open();
    void close();
    bool isOpen() const;

    int maxPacketSize() const;
    // Low level read/write functionality.
    int write( const std::basic_string<unsigned char> & to );
    int read( std::basic_string<unsigned char> & from ,int size );
    int setTimeout( int ms );

    std::basic_string<unsigned char> & dataTo();
    std::basic_string<unsigned char> & dataFrom();
protected:
    class PD;
    PD * pd;
};

#endif



