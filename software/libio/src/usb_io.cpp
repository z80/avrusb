

#include "usb_io.h"
#include <string.h>
extern "C"
{
    #include "libusb.h"
    //#include "opendevice.h"
}

inline static bool LSB()
{
    int i = 1;
    unsigned char * p = reinterpret_cast<unsigned char *>( &i );
    bool res = (p[0] != 0);
    return res;
}

class UsbIo::PD
{
public:
    PD() {}
    ~PD() {}
    libusb_context       * cxt;
    libusb_device_handle * handle;
    std::string err;
    std::string res;
    std::string output;
    int timeout;
    std::basic_string<unsigned char> dataTo,
                                      dataFrom;
    static const int VENDOR_ID;
    static const int PRODUCT_ID;
    static const int TIMEOUT;
};

const int UsbIo::PD::VENDOR_ID  = 0x16C0;
const int UsbIo::PD::PRODUCT_ID = 0x05DD;
const int UsbIo::PD::TIMEOUT    = 1000;

static const int CONTROL_REQUEST_TYPE_IN = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_ENDPOINT;
static const int CONTROL_REQUEST_TYPE_OUT = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_ENDPOINT;

static const int HID_GET_REPORT = 0x01;
static const int HID_SET_REPORT = 0x09;

UsbIo::UsbIo()
{
    pd = new PD();
    pd->handle = 0;
    pd->timeout = PD::TIMEOUT;
    libusb_init( &pd->cxt );
    libusb_set_debug( pd->cxt, 3 );
    pd->res.resize( 8 );
}

UsbIo::~UsbIo()
{
    if ( isOpen() )
        close();
    libusb_exit( pd->cxt );
    delete pd;
}

bool UsbIo::open()
{
    pd->handle = 0;
    libusb_device * * l = 0;
    int cnt = libusb_get_device_list( pd->cxt, &l );
    pd->handle = libusb_open_device_with_vid_pid( pd->cxt, PD::VENDOR_ID, PD::PRODUCT_ID );
    libusb_free_device_list( l, 1 );
    bool result = (pd->handle != 0);

    int res = libusb_set_configuration( pd->handle, 1 );
    res = libusb_claim_interface( pd->handle, 1 );
    return result;
}

void UsbIo::close()
{
    if ( isOpen() )
    {
        libusb_close( pd->handle );
        pd->handle = 0;
    }
}

bool UsbIo::isOpen() const
{
    return (pd->handle != 0);
}

int UsbIo::maxPacketSize() const
{
    return 8;
}

int UsbIo::write( const std::basic_string<unsigned char> & to )
{
    if ( !isOpen() )
    {
        if ( !open() )
            return -1;
    }
    int res = libusb_control_transfer(
                  pd->handle,
                  CONTROL_REQUEST_TYPE_OUT,
                  HID_SET_REPORT,
                  0, 0,
                  const_cast<unsigned char *>( to.data() ), to.size(), pd->timeout );
    return to.size();
    if ( res < LIBUSB_SUCCESS )
    {
        close();
        return res;
    }
    return res;
}

int UsbIo::read( std::basic_string<unsigned char> & from ,int size )
{
    if ( !isOpen() )
    {
        if ( !open() )
            return -1;
    }
    from.resize( size );
    int res = libusb_control_transfer(
                  pd->handle,
                  CONTROL_REQUEST_TYPE_IN,
                  HID_GET_REPORT,
                  0, 0,
                  const_cast<unsigned char *>( from.data() ), from.size(), pd->timeout );
    if ( res < LIBUSB_SUCCESS )
    {
        close();
        return res;
    }
    return res;
}



int UsbIo::setTimeout( int ms )
{
    pd->timeout = ms;
    return 0;
}

std::basic_string<unsigned char> & UsbIo::dataTo()
{
    return pd->dataTo;
}

std::basic_string<unsigned char> & UsbIo::dataFrom()
{
    return pd->dataFrom;
}









