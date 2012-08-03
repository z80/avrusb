
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>     // for _delay_ms()

#include <avr/pgmspace.h>   // required by usbdrv.h
#include "usbdrv.h"

#define CMD_VERSION 0
#define CMD_LED   1
#define CMD_TIMER 2

#define VERSION "1.0.0"

#define LED_PORT PORTD
#define LED_DDR  DDRD
#define LED0     (1<<0)
#define LED1     (1<<1)

// Описание буфера.
#define BUFFER_SIZE 128


usbMsgLen_t usbFunctionSetup( uchar data[8] );
void setLed( unsigned char val );
void initAdc(void);
void initTimer(void);
void setTimer( unsigned short period, unsigned char div );
uchar queryBuffer( unsigned char * ptr );
// Буфер.
unsigned short g_buffer[ BUFFER_SIZE ];
unsigned char  g_timeout = 200;
unsigned short g_accum = 0;
unsigned short g_accumCnt = 0;
unsigned char  g_to   = 0;
unsigned char  g_from = 0;
unsigned char  g_cnt  = 0;
// Дополнительный делитель для таймера, чтобы уменьшить частоту.

void __attribute__((noreturn)) main( void )
{
    cli();
    wdt_enable(WDTO_1S);
    // Initializing hardware
    // Leds
    LED_DDR  |= LED0 | LED1;
    LED_PORT |= 0b00000000;

    initTimer();
    initAdc();

    // USB initialization.
    usbInit();
    usbDeviceDisconnect();  // enforce re-enumeration, do this while interrupts are disabled!
    unsigned char b = 250;
    while ( b-- )
    {
        _delay_ms( 1 );
        wdt_reset();
    }
    usbDeviceConnect();
    sei();

    for ( ;; )
    {
        // main event loop
        usbPoll();
        wdt_reset();
        //_delay_ms( 10 );
    }
}

usbMsgLen_t usbFunctionSetup( uchar data[8] )
{
    usbRequest_t    * rq = (void *)data;
    static uchar    dataBuffer[8];  // buffer must stay valid when usbFunctionSetup returns
    usbMsgPtr = dataBuffer;
    if ( rq->bRequest == CMD_LED )
    {
        setLed( rq->wValue.bytes[0] );
    }
    else if ( rq->bRequest == CMD_TIMER )
    {
        setTimer( rq->wValue.word, rq->wIndex.bytes[0] );
    }
    else
    {
        // Data return.
        unsigned char cnt;
        cnt = queryBuffer( (uchar *)dataBuffer );
        cnt <<= 1; // Bytes number is twice greater.
        return cnt;
    }

    return 0;
}

void setLed( unsigned char val )
{
    LED_PORT = (LED_PORT & (~(LED0|LED1))) | val;
}

void initAdc(void)
{
    // Clock prescaler 128 with clk=12MHz ADC freq is withing 50-200kHz.
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // Ref voltage source AVcc.
    ADMUX = (1 << REFS0);
    // Free running mode on.
    ADCSRA |= (1 << ADFR);
    // Right aligned result
    ADMUX &= ~(1 << ADLAR);
    // ADC on
    ADCSRA |= (1 << ADEN);
    // Start measures (start convertion | interrupt enable).
    ADCSRA |= (1 << ADSC) | (1 << ADIE );
}

void initTimer(void)
{
    TCCR1A = (1<<WGM21); // Reset At OCR1 match, timer off.
    TCCR1B = (1<<WGM21);
    TIMSK &= ~( (1<<OCIE1A) | (1<<TICIE1) | (1<<OCIE1B) | (1<<TOIE1));
    TIMSK |= (1<<OCIE1A);
    TCNT1 = 0;
    setTimer( 12000, 5 );
}

void setTimer( unsigned short period, unsigned char div )
{
    TCCR1B &= ~7;
    TCNT1 = 0;
    // 0 - off
    // 1 - clk
    // 2 - clk/8
    // 3 - clk/64
    // 4 - clk/256
    // 5 - clk/1024
    OCR1A = period;
    TCCR1B |= (div & 7);
}

uchar queryBuffer( unsigned char * ptr )
{
    const uchar bufSz = 4;
    unsigned short * out = (unsigned short *)ptr;
    uchar i;
    // Mask compare interrupt
    TIMSK &= ~(1<<OCIE1A);
    for ( i=0; (i<g_cnt)&&(i<bufSz); i++ )
    {
        out[i] = g_buffer[ g_from ];
        g_from = ( g_from + 1 ) % BUFFER_SIZE;
    }
    g_cnt -= i;
    // Unmask compare interrupt
    TIMSK |= (1<<OCIE1A);
    return i;
}

ISR(TIMER1_COMPA_vect)
{
    // For debug
    if ( LED_PORT & 2 )
        LED_PORT &= (~2);
    else
        LED_PORT |= 2;
    if ( !g_accumCnt )
        return;
    unsigned short res = g_accum / g_accumCnt;
    g_accum    = 0;
    g_accumCnt = 0;
    g_buffer[g_to] = res;
    g_to  = ( g_to + 1 )  % BUFFER_SIZE;
    g_cnt = ( g_cnt + 1 ) % BUFFER_SIZE;
}

ISR(ADC_vect)
{
    g_accum += ADCW;
    g_accumCnt++;
    #define MAX_ACCUM_VALUE (65535 - 1023)
    if ( g_accum > MAX_ACCUM_VALUE )
    {
        g_accum >>= 1;
        g_accumCnt >>= 1;
    }
    if ( LED_PORT & 1 )
        LED_PORT &= (~1);
    else
        LED_PORT |= 1;
}









