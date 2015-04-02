/*
                         ''~``
                        ( o o )
+------------------.oooO--(_)--Oooo.------------------+
|                             						  |
|				Written by Joseph Fortune			  |
|						3/12/15						  |
|                        							  |
|                    .oooO         					  |
|                    (   )   Oooo.  		  		  |
+---------------------\ (----(   )--------------------+
                       \_)    ) /
                             (_/
*/

#ifndef NRF24L01_H
#define NRF24L01_H

#include <avr/io.h>
#include <util/delay.h>

// ===================================== USER SETTINGS =====================================

// *** Change these macros to match your specific setup ***

// User-defined SPI library
#include "SPI.h"

// User-defined SPI function - Link this to your SPI or USI sending function
// --the function must send a byte from the argument, while returning a byte from the SPI exchange
#define SPI_WRITE(x) writeSPI(x)

// Specifics for the pins the NRF is connected to
#define NRF_PORT		PORTB
#define NRF_DDR			DDRB
#define NRF_CE			PB1
#define NRF_CSN			PB2
#define NRF_IRQ_PORT	PORTD
#define NRF_IRQ_PIN		PIND
#define NRF_IRQ			PD2

// TX/RX Address Length (5 bytes MAX)
// -- Changing this value does not change the setting in the NRF itself
// -- but if you manually change the setting, this MACRO must be updated
// -- to reflect the new length
#define NRF_ADDR_LEN 5

// Payload Size
#define NRF_PAYLOAD_SIZE 4

// General Settings
#define NRF_SETTINGS (1 << MASK_TX_DS) | (1 << MASK_MAX_RT) | (1 << EN_CRC)

// =========================================================================================

// SPI Commands
#define R_REGISTER 			0x00
#define W_REGISTER 			0x20
#define R_RX_PAYLOAD 		0x61
#define W_TX_PAYLOAD		0xa0
#define FLUSH_TX			0xe1
#define FLUSH_RX			0xe2
#define REUSE_TX_PL 		0xe3
#define R_RX_PL_WID			0x60
#define W_ACK_PAYLOAD		0xa8
#define W_TX_PAYLOAD_NO_ACK 0xb0
#define NOP					0xff

// Register Map
#define CONFIG		0x00
#define EN_AA		0x01
#define EN_RXADDR	0x02
#define SETUP_AW	0x03
#define SETUP_RETR	0x04
#define RF_CH		0x05
#define RF_SETUP	0x06
#define STATUS		0x07
#define OBSERVE_TX	0x08
#define	RPD			0x09
#define	RX_ADDR_P0	0x0a
#define RX_ADDR_P1	0x0b
#define	RX_ADDR_P2	0x0c
#define RX_ADDR_P3	0x0d
#define RX_ADDR_P4	0x0e
#define RX_ADDR_P5	0x0f
#define TX_ADDR		0x10
#define RX_PW_P0	0x11
#define	RX_PW_P1	0x12
#define RX_PW_P2	0x13
#define	RX_PW_P3	0x14
#define RX_PW_P4	0x15
#define RX_PW_P5	0x16
#define FIFO_STATUS	0x17
#define	DYNPD		0x1c
#define FEATURE		0x1d

// Config Register Mnemonics
#define MASK_RX_DR 	6
#define MASK_TX_DS 	5
#define MASK_MAX_RT 4
#define EN_CRC		3
#define CRCO		2
#define PWR_UP		1
#define PRIM_RX		0

// Status Register Mnemonics
#define RX_DR 6
#define TX_DS 5
#define MAX_RT 4
#define TX_FULL 0

// Macro Functions
#define NRF_CE_HIGH() 	NRF_PORT |= (1 << NRF_CE);
#define NRF_CE_LOW() 	NRF_PORT &= ~(1 << NRF_CE);
#define NRF_CSN_HIGH()	NRF_PORT |= (1 << NRF_CSN);
#define NRF_CSN_LOW()	NRF_PORT &= ~(1 << NRF_CSN);


void nrf_init( void );

void nrf_configure( void );

void nrf_writeRegister( uint8_t reg, uint8_t *buffer, uint8_t length );

void nrf_readRegister( uint8_t reg, uint8_t *buffer, uint8_t length );

void nrf_configRegister( uint8_t reg, uint8_t value );

void nrf_powerUpTX( void );

void nrf_powerUpRX( void );

void nrf_transmit( uint8_t *buffer );

void nrf_setAddressRX( uint8_t *buffer );

void nrf_setAddressTX( uint8_t *buffer );

uint8_t nrf_getStatus( void );

void nrf_flushRX( void );

void nrf_getData( uint8_t *buffer, uint8_t size );

#endif
