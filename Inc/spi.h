#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

void spi1_gpio_init(void);
void spi1_init(void);
void spi1_write(uint8_t *data, uint32_t data_size);
void spi1_read(uint8_t *data, uint32_t data_size);

void cs_enable(void);
void cs_disable(void);

#endif /* SPI_H_ */
