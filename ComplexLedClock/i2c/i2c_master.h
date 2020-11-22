#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#define I2C_READ 1
#define I2C_WRITE 0

#include <avr/io.h>

void i2c_init();
uint8_t i2c_start(uint8_t address, uint8_t mode);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(uint8_t *data);
uint8_t i2c_read_nack(uint8_t *data);
void i2c_stop();

uint8_t wait_i2c();

#endif // I2C_MASTER_H
