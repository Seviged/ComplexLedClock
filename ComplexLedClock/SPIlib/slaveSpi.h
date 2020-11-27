/*
 * slaveSpi.h
 *
 * Created: 24.11.2020 1:53:52
 *  Author: Seviged
 */ 


#ifndef SLAVESPI_H_
#define SLAVESPI_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

enum SPI_STATE{
	READ_REQUEST,
	PARSE,
	WRITE_RESPONSE
	};

void initSlaveSpi();

void parseBuffer();
	




#endif /* SLAVESPI_H_ */