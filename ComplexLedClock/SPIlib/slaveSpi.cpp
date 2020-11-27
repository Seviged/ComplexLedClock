/*
 * slaveSpi.cpp
 *
 * Created: 24.11.2020 1:54:05
 *  Author: Seviged
 */ 
#include "SPIlib/slaveSpi.h"

#define SPI_PIN_MISO	4		//MISO PIN
#define SPI_DDR			DDRB	//SPI on PORTB




#define reqLen 10
#define respLen 10


SPI_STATE spiState = READ_REQUEST;
static volatile uint8_t requestHead = 0;
static volatile uint8_t responseHead = 0;
static volatile uint8_t responseLen = 0;
static volatile uint8_t spiRequest[reqLen];
static volatile uint8_t spiResponse[respLen];

void initSlaveSpi()
{
	
	// Set MISO output, all others input
	SPI_DDR |= (1<<SPI_PIN_MISO);
	// set SPI enable, spi interrupts enable
	SPCR = (1<<SPE)|(1<<SPIE);
	
	// Clear SPI Interrupt by reading SPSR and SPDR
	uint8_t dump=0x00;
	dump=SPSR;
	dump=SPDR;
	(void)dump;
	SPDR=0x00; // Set SPDR to 0x00

}

void parseBuffer()
{
	if(spiState == PARSE)
	{
		for (int i = 0; i < requestHead; ++i)
		{
			spiResponse[i] = spiRequest[i];
			spiRequest[i] = 0x00;
			responseLen = i + 1;
		}
		
		requestHead = 0;
		responseHead = 0;
		spiState = WRITE_RESPONSE;
	}
}

ISR(SPI_STC_vect)
/*************************************************************************
Function: SPI interrupt
Purpose:  called when the SS pin has been put low
**************************************************************************/
{
	/* SPI Slave */
	uint8_t temp = SPDR;
	if(spiState == READ_REQUEST)
	{
		if(temp != 0xff)
		{
			spiRequest[requestHead] = temp;
			++requestHead;
		}
		
		if (temp == 0xfe)
		{
			spiState = PARSE;
		}
		SPDR = 0xf0;
	}
	else if (spiState == PARSE)
	{
		SPDR = 0xf1;
	}
	else if (spiState == WRITE_RESPONSE)
	{
		if(responseHead < responseLen)
		{
			SPDR = spiResponse[responseHead];
			++responseHead;
		}
		else
		{
			SPDR = 0xf2;
			spiState = READ_REQUEST;
		}
	}
	else
	{
		SPDR = 0xf3;
	}
	
	
}
