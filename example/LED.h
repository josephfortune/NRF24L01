// Written by Joseph Fortune
// 3/13/15

#ifndef LED_H
#define LED_H

#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT 	PORTB
#define LED_DDR 	DDRB
#define LED_PIN 	PB0

void led_init(void);
void led_blinkByte( uint8_t byte );
void led_blinkFast( void );
void led_blinkSlow( void );

#endif
