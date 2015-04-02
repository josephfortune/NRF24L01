#include "LED.h"

void led_init(void)
{
	LED_DDR |= (1 << LED_PIN);
}

void led_blinkByte( uint8_t byte )
{
	for ( int i = 0; i < 3; i++ )
	{
		LED_PORT |= (1 << LED_PIN);
		_delay_ms(100);
		LED_PORT &= ~(1 << LED_PIN);
		_delay_ms(100);
	}	
	

	for ( int i = 7; i >= 0; i-- )
	{
		if ((1 << i) & byte)
			LED_PORT |= (1 << LED_PIN);
		else
			LED_PORT &= ~(1 << LED_PIN);
		
		_delay_ms(1000);
		LED_PORT |= (1 << LED_PIN);
		_delay_ms(100);
		LED_PORT &= ~(1 << LED_PIN);
		_delay_ms(100);

	}

	for ( int i = 0; i < 6; i++ )
	{
		LED_PORT |= (1 << LED_PIN);
		_delay_ms(100);
		LED_PORT &= ~(1 << LED_PIN);
		_delay_ms(100);
	}	
}

void led_blinkFast( void )
{
	LED_PORT |= (1 << LED_PIN);
	_delay_ms(100);
	LED_PORT &= ~(1 << LED_PIN);
	_delay_ms(100);
}

void led_blinkSlow( void )
{
	LED_PORT |= (1 << LED_PIN);
	_delay_ms(1000);
	LED_PORT &= ~(1 << LED_PIN);
	_delay_ms(1000);
}
