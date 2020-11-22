#include "TSL2581.h"
#include "../i2c/i2c_master.h"


/**************************************************************************
  Writes a register and an 8 bit value over I2C
**************************************************************************/
void WaveShare_TSL2581::Write8(byte reg, unsigned long value, byte *result)
{
    *result += i2c_start(I2C_addr, I2C_WRITE);
    *result += i2c_write(reg);
    *result += i2c_write(value & 0xFF);
    i2c_stop();
}

/**************************************************************************
  Read a register and an 8 bit value over I2C
**************************************************************************/
byte WaveShare_TSL2581::Read8(byte reg, byte *result)
{   
    *result += i2c_start(I2C_addr, I2C_WRITE);
    *result += i2c_write(reg);
    i2c_stop();
    
    uint8_t data;
    *result += i2c_start(I2C_addr, I2C_READ);
    *result += i2c_read_nack(&data);
    i2c_stop();
    
    return data;
}

/**************************************************************************
      Constructor
**************************************************************************/
WaveShare_TSL2581::WaveShare_TSL2581() {
  I2C_addr = ADDR_FLOAT;		//ADDR  -->   float
}

WaveShare_TSL2581::WaveShare_TSL2581(byte addr) {
  I2C_addr = addr;
}

/**************************************************************************
      //power on  and off
**************************************************************************/
void WaveShare_TSL2581::TSL2581_power_on(void) {
  uint8_t result = 0;
  Write8(COMMAND_CMD  | CONTROL, CONTROL_POWERON, &result);
}

/**************************************************************************
      //set timer and INTERRUPT
**************************************************************************/
void WaveShare_TSL2581::TSL2581_config(void) {
  uint8_t result = 0;
  Write8(COMMAND_CMD | TIMING, INTEGRATIONTIME_400MS, &result);  //400MS
  Write8(COMMAND_CMD | CONTROL, ADC_EN | CONTROL_POWERON, &result); //
  Write8(COMMAND_CMD | INTERRUPT, INTR_INTER_MODE, &result);//Every ADC cycle generates interrupt
  Write8(COMMAND_CMD | ANALOG, GAIN_16X, &result);//GAIN = 16
}
void WaveShare_TSL2581::Reload_register(void)
{
  uint8_t result = 0;
  Write8(COMMAND_CMD | TRANSACTION_SPECIAL | SPECIAL_FUN_INTCLEAR, SPECIAL_FUN_INTCLEAR, &result);
  Write8(COMMAND_CMD | CONTROL, ADC_EN | CONTROL_POWERON, &result);
}
/**************************************************************************
      //read i2c ID   || ID = 80 ||
**************************************************************************/
byte WaveShare_TSL2581::TSL2581_Read_ID(void) {
    uint8_t result = 0;
    byte data = Read8(COMMAND_CMD | TRANSACTION | ID, &result);
    if(result) return 0;
    
    return data;
}

/*********************************************************
   //SET_Interrupt_Threshold
**********************************************************/
void WaveShare_TSL2581::SET_Interrupt_Threshold(unsigned long low, unsigned long high)
{
  uint8_t result = 0;
  unsigned int DataLLow, DataLHigh, DataHLow, DataHHigh;
  DataLLow = low % 256;
  DataLHigh = low / 256;
  Write8(COMMAND_CMD | THLLOW, DataLLow, &result);
  Write8(COMMAND_CMD | THLHIGH, DataLHigh, &result);

  DataHLow = high % 256;
  DataHHigh = high / 256;
  Write8( COMMAND_CMD | THHLOW, DataHLow, &result);
  Write8(COMMAND_CMD | THHHIGH, DataHHigh, &result);

}
void WaveShare_TSL2581::TSL2581_Read_Channel()
{
  //delay(450);

  uint8_t result = 0;
  
  word low1, high1;
  word low2, high2;
  low1 = Read8( COMMAND_CMD | TRANSACTION | DATA0LOW, &result);
  high1 = Read8( COMMAND_CMD | TRANSACTION | DATA0HIGH, &result);
  
  low2 = Read8( COMMAND_CMD | TRANSACTION | DATA1LOW, &result);
  high2 = Read8( COMMAND_CMD | TRANSACTION | DATA1HIGH, &result);
  
  if(result) return;
  
  ch0 = high1 * 256 + low1;
  ch1 = high2 * 256 + low2;
}

// Arguments: unsigned int iGain - gain, where 0:1X, 1:8X, 2:16X, 3:128X
// unsigned int tIntCycles - INTEG_CYCLES defined in Timing Register
unsigned long WaveShare_TSL2581::calculateLux(unsigned int iGain, unsigned int tIntCycles)
{
  unsigned long chScale0;
  unsigned long chScale1;
  unsigned long channel1;
  unsigned long channel0;
  unsigned long temp;
  unsigned long ratio1 = 0;
  unsigned long ratio	;
  unsigned long lux_temp;
  unsigned int b, m;

  // No scaling if nominal integration (148 cycles or 400 ms) is used
  if (tIntCycles == NOM_INTEG_CYCLE)
    chScale0 = 65536;
    //chScale0 = (1 << (CH_SCALE));
  else
    chScale0 = (NOM_INTEG_CYCLE << CH_SCALE) / tIntCycles;
  switch (iGain)
  {
    case 0: // 1x gain
      chScale1 = chScale0; // No scale. Nominal setting
      break;
    case 1: // 8x gain
      chScale0 = chScale0 >> 3; // Scale/multiply value by 1/8
      chScale1 = chScale0;
      break;
    case 2: // 16x gain
      chScale0 = chScale0 >> 4; // Scale/multiply value by 1/16
      chScale1 = chScale0;
      break;
    case 3: // 128x gain
      chScale1 = chScale0 / CH1GAIN128X;
      chScale0 = chScale0 / CH0GAIN128X;
      break;
    default:
      chScale1 = chScale0;
      break;
  }
  // scale the channel values
  channel0 = (ch0 * chScale0) >>  CH_SCALE;
  channel1 = (ch1 * chScale1) >>  CH_SCALE;

  // find the ratio of the channel values (Channel1/Channel0)
  if (channel0 != 0)
    ratio1 = (channel1 << (RATIO_SCALE + 1)) / channel0;

  ratio = (ratio1 + 1) >> 1;	  									 // round the ratio value

  if (ratio <= K1C)
  {
    b = B1C;
    m = M1C;
  }
  else if (ratio <= K2C)
  {
    b = B2C;
    m = M2C;
  }
  else if (ratio <= K3C)
  {
    b = B3C;
    m = M3C;
  }
  else if (ratio <= K4C)//276
  {
    b = B4C;
    m = M4C;
  }
  else if (ratio > K5C)//276
  {
    b = B5C;
    m = M5C;
  }
  temp = ((channel0 * b) - (channel1 * m));
  //  temp += (1 << (LUX_SCALE - 1));
  temp = temp + 32768;               // round lsb (2^(LUX_SCALE-1))
  lux_temp = temp >> LUX_SCALE;			// strip off fractional portion
  return (lux_temp);		  							// Signal I2C had no errors
}
