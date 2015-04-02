#include "NRF24L01.h"

//  =========================================================
//  ||                      nrf_init					   ||
//  =========================================================
void nrf_init()
{
	// Enable outputs	
	NRF_DDR |= (1 << NRF_CSN) | (1 << NRF_CE);

	// Enable pull-up resistor on interrupt pin
	NRF_PORT |= (1 << NRF_IRQ); 

	// Start with NRF idle
	NRF_CSN_HIGH();
	NRF_CE_LOW();
 
} 

//  =========================================================
//  ||                    nrf_configure					   ||
//  =========================================================
void nrf_configure()
{ 
	// Set length of incoming payload
	nrf_configRegister( RX_PW_P0, NRF_PAYLOAD_SIZE );
	nrf_configRegister( RX_PW_P1, NRF_PAYLOAD_SIZE );

	// Set channel
	nrf_configRegister ( RF_CH, 1 ); 

	nrf_powerUpRX();
	nrf_flushRX();
 
}

//  =========================================================
//  ||                   nrf_writeRegister				   ||
//  =========================================================
void nrf_writeRegister( uint8_t reg, uint8_t *buffer, uint8_t length )
{
	NRF_CSN_LOW();

	// Send the Write Command / Register address, followed by the value
	SPI_WRITE( W_REGISTER | reg );
	for ( int i = 0; i < length; i++ )	
		SPI_WRITE( buffer[i] );

	NRF_CSN_HIGH(); 
}

//  =========================================================
//  ||                   nrf_readRegister				   ||
//  =========================================================
void nrf_readRegister( uint8_t reg, uint8_t *buffer, uint8_t length )
{
	NRF_CSN_LOW();

	SPI_WRITE( R_REGISTER | reg );
	for ( int i = 0; i < length; i++ )
		buffer[i] = SPI_WRITE( NOP );

	NRF_CSN_HIGH(); 
}

//  =========================================================
//  ||                  nrf_configRegister				   ||
//  =========================================================
void nrf_configRegister( uint8_t reg, uint8_t value )
{
	NRF_CSN_LOW();

	SPI_WRITE( W_REGISTER | reg );
	SPI_WRITE( value );

	NRF_CSN_HIGH();
}

//  =========================================================
//  ||                    nrf_powerUpTX					   ||
//  =========================================================
void nrf_powerUpTX( void )
{
	nrf_configRegister( CONFIG, NRF_SETTINGS | (1 << PWR_UP) );
}

//  =========================================================
//  ||                    nrf_powerUpRX					   ||
//  =========================================================
void nrf_powerUpRX( void )
{
	NRF_CE_LOW();
	nrf_configRegister( CONFIG, NRF_SETTINGS | (1 << PWR_UP) | (1 << PRIM_RX) );
	NRF_CE_HIGH();

	nrf_configRegister( STATUS, (1 << TX_DS) | (1 << MAX_RT) );
}

//  =========================================================
//  ||                    nrf_trasnmit					   ||
//  =========================================================
void nrf_transmit( uint8_t *buffer )
{
	// Make sure the nrf isn't already transmitting	
	uint8_t status;
	do
	{	
		status = SPI_WRITE( NOP );
	} while ( !( status & ( (1 << TX_DS) | (1 << MAX_RT) ) ) );


	NRF_CE_LOW();
	nrf_powerUpTX(); // Power up in transmit mode
	
	NRF_CSN_LOW();
	SPI_WRITE( FLUSH_TX ); // Flush the TX FIFO
	NRF_CSN_HIGH();
	_delay_ms(10);

	// Fill the TX FIFO with the outgoing data
	NRF_CSN_LOW();
	SPI_WRITE( W_TX_PAYLOAD );
	for ( int i = 0; i < NRF_PAYLOAD_SIZE; i++ )
		SPI_WRITE( buffer[i] );
	_delay_ms(10);
	NRF_CSN_HIGH();

	NRF_CE_HIGH(); // Begin transmission

}

//  =========================================================
//  ||                   nrf_setAddressRX				   ||
//  =========================================================
void nrf_setAddressRX( uint8_t *buffer )
{
	NRF_CE_LOW(); 
	nrf_writeRegister( RX_ADDR_P1, buffer, NRF_ADDR_LEN );
	NRF_CE_HIGH();
}

//  =========================================================
//  ||                   nrf_setAddressTX				   ||
//  =========================================================
void nrf_setAddressTX( uint8_t *buffer )
{
	nrf_writeRegister( TX_ADDR, buffer, NRF_ADDR_LEN );
	nrf_writeRegister( RX_ADDR_P0, buffer, NRF_ADDR_LEN );
}

//  =========================================================
//  ||                    nrf_getStatus					   ||
//  =========================================================
uint8_t nrf_getStatus( void )
{
	uint8_t status;	

	NRF_CSN_LOW();
	status = SPI_WRITE( NOP );
	NRF_CSN_HIGH();

	return status;
}

//  =========================================================
//  ||                    nrf_flushRX					   ||
//  =========================================================
void nrf_flushRX( void )
{
	NRF_CSN_LOW();
	SPI_WRITE( FLUSH_RX );
	NRF_CSN_HIGH();
}

//  =========================================================
//  ||                     nrf_getData					   ||
//  =========================================================
void nrf_getData( uint8_t *buffer, uint8_t size )
{
	NRF_CSN_LOW();
	SPI_WRITE( R_RX_PAYLOAD );
	for ( int i = 0; i < size; i++ )
		buffer[i] = SPI_WRITE( R_RX_PAYLOAD );
	NRF_CSN_HIGH();

	nrf_configRegister( STATUS, (1 << RX_DR) );
}
