#ifndef  F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>

#include "../i2c/i2c_master.h"

#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)



void i2c_init()
{
	TWBR = (uint8_t)TWBR_val;
}

uint8_t i2c_start(uint8_t address, uint8_t mode)
{
    address = address << 1;
    if(mode) address |= (1 << 0);
    
	// reset TWI control register
	TWCR = 0;
	// transmit START condition 
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	if(wait_i2c()) return 1;
	
	// check if the start condition was successfully transmitted
	if((TWSR & 0xF8) != TW_START){ return 1; }
	
	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	if(wait_i2c()) return 1;
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
	
	return 0;
}

uint8_t i2c_write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	if(wait_i2c()) return 1;
	
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK ){ return 1; }
	
	return 0;
}

uint8_t i2c_read_ack(uint8_t *data)
{
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	if(wait_i2c()) return 1;
    
    *data = TWDR;
    
	// return received data from TWDR
	return 0;
}

uint8_t i2c_read_nack(uint8_t *data)
{
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	if(wait_i2c()) return 1;
    
    *data = TWDR;
    
	// return received data from TWDR
	return 0;
}

void i2c_stop()
{
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

uint8_t wait_i2c()
{
    uint32_t breakout = 500000;
    while( !(TWCR & (1<<TWINT)) && breakout != 0)
    {
        --breakout;
    }
    
    if(breakout == 0) return 1;
    
    return 0;
}
