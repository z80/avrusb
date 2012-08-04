
#ifndef __FUNCS_H_
#define __FUNCS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>     // for _delay_ms()

#include <avr/pgmspace.h>   // required by usbdrv.h
#include "usbdrv.h"


void invoke( uchar id, uchar * io );


#endif


