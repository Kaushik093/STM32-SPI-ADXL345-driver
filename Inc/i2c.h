#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

void i2c1_init(void);
void i2c1_byte_read(char saddr, char mem_addr, char* data);
void i2c1_burst_read(char saddr, char mem_addr, int data_size, char* data);
void i2c1_burst_write(char saddr, char mem_addr, int data_size, char* data);

#endif /* I2C_H_ */
