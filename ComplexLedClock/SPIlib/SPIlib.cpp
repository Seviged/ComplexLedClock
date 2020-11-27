/*************************************************************************
	
	SPI library by Julien Delvaux

*************************************************************************/

#include "SPIlib.h"


void dumb()
{
	
}

/************************************************************************/
/* Constants and macros                                                 */
/************************************************************************/

#define SPI_ACTIVE			0 // SS Pin put Low
#define SPI_INACTIVE		1 // SS Pin put High	

/* size of RX/TX buffers */
#define SPI_RX_BUFFER_MASK ( SPI_RX_BUFFER_SIZE - 1)
#define SPI_TX_BUFFER_MASK ( SPI_TX_BUFFER_SIZE - 1)

#if ( SPI_RX_BUFFER_SIZE & SPI_RX_BUFFER_MASK )
	#error RX buffer size is not a power of 2
#endif
#if ( SPI_TX_BUFFER_SIZE & SPI_TX_BUFFER_MASK )
	#error TX buffer size is not a power of 2
#endif

#if	defined(__AVR_ATmega48A__) ||defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega88A__) || \
	defined(__AVR_ATmega88PA__) ||defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || \
	defined(__AVR_ATmega328P__)
	#define SPI_PIN_SS		2		//SS PIN
	#define SPI_PIN_MOSI	3		//MOSI PIN
	#define SPI_PIN_MISO	4		//MISO PIN
	#define SPI_PIN_SCK		5		//SCK PIN
	#define SPI_DDR			DDRB	//SPI on PORTB
	#define SPI_PORT		PORTB	//SPI on PORTB
#else
	#error "no SPI definition for MCU available"
#endif

/************************************************************************/
/* Global variable                                                      */
/************************************************************************/

static volatile uint8_t SPI_TxBuf[SPI_TX_BUFFER_SIZE];
static volatile uint8_t SPI_RxBuf[SPI_RX_BUFFER_SIZE];
static volatile uint8_t SPI_TxHead;
static volatile uint8_t SPI_TxTail;
static volatile uint8_t SPI_RxHead;
static volatile uint8_t SPI_RxTail;

#if defined(SPI_SLAVE_ENABLED)
	static volatile uint8_t SPI_SPDR;
	#define SPI_SPDR_EMPTY	0
	#define SPI_SPDR_FULL	1
#endif

ISR(SPI_STC_vect)
/*************************************************************************
Function: SPI interrupt
Purpose:  called when the SS pin has been put low
**************************************************************************/
{
	uint16_t tmphead=0;
	uint16_t tmptail=0;
	/* SPI Slave */
		
	//RECEIVE
	// calculate buffer index
	tmphead = ( SPI_RxHead + 1) & SPI_RX_BUFFER_MASK;
	if ( tmphead == SPI_RxTail ) {
		// error: receive buffer overflow
					
	} 
	else 
	{
		// store new index
		SPI_RxHead = tmphead;
		// store received data in buffer
		SPI_RxBuf[tmphead] = SPDR;
	}

	// SEND
	if ( SPI_TxHead != SPI_TxTail) 
	{
		// calculate and store new buffer index
		tmptail = (SPI_TxTail + 1) & SPI_TX_BUFFER_MASK;
		SPI_TxTail = tmptail;
		// get one byte from buffer and write it to UART
		SPDR = SPI_TxBuf[tmptail];  //start transmission
	} 
	else
	{
		SPDR=0x00;
	}

}


#if defined (SPI_SLAVE_ENABLED)
/*************************************************************************
Function: spi_slave_init()
Purpose:  Initialize SPI in Slave Mode
Input:    none
Returns:  none
**************************************************************************/
void spi_slave_init(){

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
	
	SPI_SPDR = SPI_SPDR_EMPTY;

}

#endif
/*************************************************************************
Function: spi_close()
Purpose:  Close SPI, flush and clear any received datas
Input:    none
Returns:  none
**************************************************************************/
void spi_close(){
	
	spi_flush();
	
	SPCR = (0x00);
	SPI_DDR &= ~(1<<SPI_PIN_SS);
	SPI_PORT&= ~(1<<SPI_PIN_SS);
	
}

/*************************************************************************
Function: spi_getc()
Purpose:  return byte from ringbuffer
Returns:  byte:  received byte from ringbuffer
**************************************************************************/
uint8_t spi_getc()
{
	uint16_t tmptail;
	uint8_t data;

	if ( SPI_RxHead == SPI_RxTail ) {
		/* no data available */
	}

	/* calculate /store buffer index */
	tmptail = (SPI_RxTail + 1) & SPI_RX_BUFFER_MASK;
	SPI_RxTail = tmptail;

	/* get data from receive buffer */
	data = SPI_RxBuf[tmptail];

	return data;
}

/*************************************************************************
Function: spi_putc()
Purpose:  write byte to ringbuffer for transmitting via SPI
Input:    byte to be transmitted
Returns:  none
**************************************************************************/
void spi_putc(uint8_t data)
{
	uint16_t tmphead;

	tmphead  = (SPI_TxHead + 1) & SPI_TX_BUFFER_MASK;
	
	#if defined (SPI_SLAVE_ENABLED)
	// If no char in SPDR -> fill directly
	if (SPI_TxHead == SPI_TxTail && SPI_SPDR==SPI_SPDR_EMPTY){
		SPDR = data;
		SPI_SPDR = SPI_SPDR_FULL;
	}
	else if (tmphead != SPI_TxTail){
		SPI_TxBuf[tmphead] = data;
		SPI_TxHead = tmphead;
	}
	#elif defined (SPI_MASTER_ENABLED)
	if (tmphead != SPI_TxTail){
		SPI_TxBuf[tmphead] = data;
		SPI_TxHead = tmphead;
	}
	#endif
	
}

/*************************************************************************
Function: spi_puts()
Purpose:  transmit string to SPI
Input:    string to be transmitted
Returns:  none
**************************************************************************/
void spi_puts(const char *s )
{
	while (*s) {
		spi_putc(*s++);
	}

}

/*************************************************************************
Function: spi_available()
Purpose:  Determine the number of bytes waiting in the receive buffer
Input:    None
Returns:  Integer number of bytes in the receive buffer
**************************************************************************/
uint16_t spi_available()
{
	return (SPI_RX_BUFFER_SIZE + SPI_RxHead - SPI_RxTail) & SPI_RX_BUFFER_MASK;
}

/*************************************************************************
Function: spi_flush()
Purpose:  Flush bytes waiting the receive buffer.  Acutally ignores them.
Input:    None
Returns:  None
**************************************************************************/
void spi_flush()
{
	SPI_RxHead = SPI_RxTail;
}