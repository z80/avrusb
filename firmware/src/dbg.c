
#include "dbg.h"
#include <avr/io.h>

#define LED_PORT PORTD
#define LED_DDR  DDRD
#define LED0     (1<<0)
#define LED1     (1<<1)

void initLeds( void )
{
    LED_DDR  |= LED0 | LED1;
    LED_PORT |= 0b00000000;
}

void setLeds( unsigned char val )
{
    LED_PORT = (LED_PORT & (~(LED0|LED1))) | val;
}

void setLed0( unsigned char val )
{
    LED_PORT = (LED_PORT & (~(LED0))) | val;
}

void setLed1( unsigned char val )
{
    LED_PORT = (LED_PORT & (~(LED1))) | val;
}

void blinkLed0( void )
{
    uint8_t led = (LED_PORT & LED0) ? 0 : LED0;
    LED_PORT = (LED_PORT & (~(LED0))) | led;
}

void blinkLed1( void )
{
    uint8_t led = (LED_PORT & LED1) ? 0 : LED1;
    LED_PORT = (LED_PORT & (~(LED1))) | led;
}




