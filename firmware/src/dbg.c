
#include "dbg.h"
#include <avr/io.h>

#define LED_PORT PORTD
#define LED_DDR  DDRD
#define LED0     (1<<0)
#define LED1     (1<<1)

void initLeds( void )
{
    #ifdef DEBUG_LEDS_ATMEGA8
        LED_DDR  |= LED0 | LED1;
        LED_PORT |= 0b00000000;
    #endif
}

void setLeds( unsigned char val )
{
    #ifdef DEBUG_LEDS_ATMEGA8
        LED_PORT = (LED_PORT & (~(LED0|LED1))) | val;
    #endif
}

void setLed0( unsigned char val )
{
    #ifdef DEBUG_LEDS_ATMEGA8
        LED_PORT = (LED_PORT & (~(LED0))) | val;
    #endif
}

void setLed1( unsigned char val )
{
    #ifdef DEBUG_LEDS_ATMEGA8
        LED_PORT = (LED_PORT & (~(LED1))) | val;
    #endif
}

void blinkLed0( void )
{
    #ifdef DEBUG_LEDS_ATMEGA8
        uint8_t led = (LED_PORT & LED0) ? 0 : LED0;
        LED_PORT = (LED_PORT & (~(LED0))) | led;
    #endif
}

void blinkLed1( void )
{
    #ifdef DEBUG_LEDS_ATMEGA8
        uint8_t led = (LED_PORT & LED1) ? 0 : LED1;
        LED_PORT = (LED_PORT & (~(LED1))) | led;
    #endif
}




