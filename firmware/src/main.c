
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>     // for _delay_ms()

#include <avr/pgmspace.h>   // required by usbdrv.h
#include "usbdrv.h"
#include "cpu_io.h"
#include "dbg.h"

usbMsgLen_t usbFunctionSetup( uchar data[8] );

void __attribute__((noreturn)) main( void )
{
    cli();
    initLeds();

    wdt_enable( WDTO_1S );

    // USB initialization.
    usbInit();
    usbDeviceDisconnect();  // enforce re-enumeration, do this while interrupts are disabled!
    unsigned char b = 150;
    while ( b-- )
    {
        _delay_ms( 1 );
        wdt_reset();
    }
    cpuIoInit();

    usbDeviceConnect();
    sei();

    for ( ;; )
    {
        // main event loop
        usbPoll();
        wdt_reset();
        cpuIoPoll();
        //_delay_ms( 10 );
    }
}

usbMsgLen_t usbFunctionSetup( uchar data[8] )
{
    usbRequest_t    * rq = (void *)data;
    static uchar    dataBuffer[8];  // buffer must stay valid when usbFunctionSetup returns
    usbMsgPtr = dataBuffer;
    if( (rq->bmRequestType & USBRQ_TYPE_MASK) != 0 /*== USBRQ_TYPE_CLASS*/ )
    {    // HID class request
        if(rq->bRequest == USBRQ_HID_GET_REPORT)
        {
            // wValue: ReportType (highbyte), ReportID (lowbyte)
            return USB_NO_MSG;  // use usbFunctionRead() to obtain data
        }
        else if(rq->bRequest == USBRQ_HID_SET_REPORT)
        {
            return USB_NO_MSG;  // use usbFunctionWrite() to receive data from host
        }
    }
    return 0;
    //return USB_NO_MSG;
}

uchar usbFunctionRead(uchar *data, uchar len)
{
    /*if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_read_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return len;*/
    //uchar i;
    //for ( i=0; i<len; i++ )
    //    data[i] = 'a' + i;
    blinkLed1();
    cpuIoPop( data, len );
    return len;
}

uchar usbFunctionWrite( uchar *data, uchar len )
{
    /*
    if(bytesRemaining == 0)
        return 1;               // end of transfer
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_write_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return bytesRemaining == 0; // return 1 if this was the last chunk
    */
    // Return 1 if don't know if data should come or not.
    if ( ( data[0] == 0 ) && ( data[1] == 1 ) )
        blinkLed0();
    cpuIoPush( data, len );
    return 1;
}








