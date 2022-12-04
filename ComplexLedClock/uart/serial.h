/*
 * Serial.h
 *
 * Created: 04.12.2022 3:50:10
 *  Author: Seviged
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#include "../ds3231/DS3231.h"
#define BUFFER_LEN 20


	void startSerial(uint32_t baudRate, uint32_t fcpu, RealTimeClock *rtc);
	void processSerial();
	
	void _clearBuffer();
	bool _checkMessage();
	void _parseCommand();
	void _addSymbolToBuffer(uint8_t b);

#endif /* SERIAL_H_ */