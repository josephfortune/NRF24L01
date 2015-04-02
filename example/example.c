#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "SPI.h"
#include "NRF24L01.h"
#include "LED.h"

/* This example code is intended work as the client with the Arduino's MIRF library Server sketch.
 * Every second, the NRF sends a transmission, and the result triggers an interrupt, blinking the
 * results.
 *
 * 3 Fast blinks -> Message recieved
*/

ISR( INT0_vect )
{
	cli();		
	uint8_t status;
	status = nrf_getStatus();

	if ( status & (1 << RX_DR ) )
	{
		led_blinkFast();
		_delay_ms(10);
		led_blinkFast();
		_delay_ms(10);
		led_blinkFast();
		_delay_ms(10);
		led_blinkFast();
		_delay_ms(10);
		
		nrf_flushRX();
		nrf_configRegister( STATUS, (1 << RX_DR)  );
	}

	sei();
}

void initInterrupt( void )
{
	// Enable NRF interrupt	
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC00);
	sei();
}
int main( void )
{
	/* Special note
		 because the NRF is using PB2, the NRF's DDR must be set before the SPI is enabled. If SPI sees PB2 as input, and input is floating or 			 pulled low, the ATMEGA is sent into slave mode and the SPIF flag which indicated a complete SPI transfer is never set, locking up the 			 system.
	*/
	nrf_init();
	led_init();
	initSPI();
	initInterrupt();
	_delay_ms(100);	

	nrf_configure();	

	uint8_t addressTX[5] = { 's', 'e', 'r', 'v', '1' };
	uint8_t addressRX[5] = { 'c', 'l', 'i', 'e', '1' };
	nrf_setAddressRX( addressRX );
	nrf_setAddressTX( addressTX );
	
	uint8_t payload[] = { 't', 'e', 's', 't' };
	

	while (1)
	{			
		nrf_powerUpTX();
		_delay_ms(1); // Give NRF a moment to switch modes

		nrf_transmit( payload );
		nrf_powerUpRX();

		_delay_ms(1000);

	}

}
