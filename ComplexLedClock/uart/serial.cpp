/*
 * serial.cpp
 *
 * Created: 04.12.2022 3:50:49
 *  Author: Seviged
 */ 

#define USART0_LARGE_BUFFER

//#ifndef F_CPU
//#define F_CPU 8000000UL
//#endif

#include "uart.h"
#include "serial.h"
//#include <util/delay.h>

uint16_t serialReceiveSymbol;
uint8_t buffer[BUFFER_LEN];
uint8_t _pos = 0;
uint8_t _lastSymbol = 0;
uint8_t rxStatus = 0;
RealTimeClock *_rtc;

void startSerial(uint32_t baudRate, uint32_t fcpu, RealTimeClock *rtc)
{
	_clearBuffer();
	uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(baudRate, fcpu));
	_rtc = rtc;
}

void processSerial()
{
    serialReceiveSymbol = uart_getc();
    if (serialReceiveSymbol & UART_NO_DATA)
    {
        /*
         * No data available from UART
         */
    }
    else
    {
        /*
            * New data available from UART
            * Check for Frame or Overrun error
            */
        if (serialReceiveSymbol & UART_FRAME_ERROR)
        {
            /* Framing Error detected, i.e no stop bit detected */
            //uart_puts_P("UART Frame Error: ");
        }
        if (serialReceiveSymbol & UART_OVERRUN_ERROR)
        {
            /*
                * Overrun, a character already present in the UART UDR register was
                * not read by the interrupt handler before the next character arrived,
                * one or more received characters have been dropped
                */
            //uart_puts_P("UART Overrun Error: ");
        }
        if (serialReceiveSymbol & UART_BUFFER_OVERFLOW)
        {
            /*
                * We are not reading the receive buffer fast enough,
                * one or more received character have been dropped
                */
            //uart_puts_P("Buffer overflow error: ");
        }
        /*
            * Send received character back
            */
        //uart_putc((uint8_t)serialReceiveSymbol);
		_addSymbolToBuffer((uint8_t)serialReceiveSymbol);
    }	
}

void _clearBuffer()
{
	_pos = 0;
	for(uint8_t i = 0; i < BUFFER_LEN; ++i)
	{
		buffer[i] = 0;
	}
}

bool _checkMessage()
{
	//prefix AA
	//suffix XX
	
	if(buffer[0] == 'A' && buffer[1] == 'A')
	{
		
		if(_pos > 2 && buffer[_pos-1] == 'X' && buffer[_pos-2] == 'X')
		{
			uint8_t messageCrc8 = buffer[_pos - 3];
			uint8_t crc8 = 0;
			
			for(uint8_t i = 0; i < _pos - 3; ++i)
			{
				crc8 += buffer[i];
				
			}
			crc8 = 255 - crc8;
			uart_putc(crc8);
			uart_putc(messageCrc8);
			if(crc8 == messageCrc8)
			{
				uart_putc('z');
				return true;	
			}
		}
	}
	
	return false;
}

void _parseCommand()
{
	if(buffer[2] == 'S' && buffer[3] == 'T')
	{
		uint8_t hour = (buffer[4] - 48) * 10;
		hour += (buffer[5] - 48);
		
		//6 == :
		
		uint8_t minute = (buffer[7] - 48) * 10;
		minute += (buffer[8] - 48);
		
		//9 == :
		
		uint8_t seconds = (buffer[10] - 48) * 10;
		seconds += (buffer[11] - 48);
		
		_rtc->setTimeToRtc(hour, minute, seconds);
	}
	
	_clearBuffer();
}

void _addSymbolToBuffer(uint8_t b)
{	
	if(_lastSymbol == 'A' && b == 'A')
	{
		rxStatus = 1;
		_pos = 0;
		buffer[_pos] = 'A';
		_pos++;
	}
	
	if (rxStatus == 1)
	{
		buffer[_pos] = b;
		_pos++;
	}
	
	if(_pos >= BUFFER_LEN)
	{
		_clearBuffer();
		rxStatus = 0;
	}
	
	if(rxStatus == 1 && _lastSymbol == 'X' && b == 'X')
	{
		if(_checkMessage())
		{
			_parseCommand();
		}
		else
		{
			_clearBuffer();
		}
	}
	
	_lastSymbol = b;
}

