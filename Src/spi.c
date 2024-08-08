#include "stm32f4xx.h"

#define GPIOAEN		(1U<<0)
#define SPI1_CLK_EN	(1U<<12)

#define SPI_BUSY	(1U<<7)
#define TX_EMPTY	(1U<<1)
#define RX_NEMPTY	(1U<<0)


void spi1_gpio_init(void){

	//Enable clock access to GPIOA
	RCC->AHB1ENR |=GPIOAEN;

	//Set PA4,PA5,PA6,PA7 to alternate function mode

	GPIOA->MODER &= ~(1U<<10);
	GPIOA->MODER |= (1U<<11);

	GPIOA->MODER &= ~(1U<<12);
	GPIOA->MODER |= (1U<<13);

	GPIOA->MODER &= ~(1U<<14);
	GPIOA->MODER |= (1U<<15);

	//Set PA4 to output mode
	GPIOA->MODER |=(1U<<8);
	GPIOA->MODER &=~(1U<<9);

	//Set alternate function AF05 (0101: AF5) for the above pins

	// PA5
	GPIOA->AFR[0] |= (1U << 20);
	GPIOA->AFR[0] &= ~(1U << 21);
	GPIOA->AFR[0] |= (1U << 22);
	GPIOA->AFR[0] &= ~(1U << 23);

	// PA6
	GPIOA->AFR[0] |=(1U<<24);
	GPIOA->AFR[0] &= ~(1U<<25);
	GPIOA->AFR[0] |=(1U<<26);
	GPIOA->AFR[0] &= ~(1U<<27);

	// PA7
	GPIOA->AFR[0] |=(1U<<28);
	GPIOA->AFR[0] &= ~(1U<<29);
	GPIOA->AFR[0] |=(1U<<30);
	GPIOA->AFR[0] &= ~(1U<<31);


}

void spi1_init(void){

	//Enable clock for SPI1
	RCC->APB2ENR |= SPI1_CLK_EN;

	//Set SPI1 to master
	SPI1->CR1 |= (1U<<2);

	//Set MSB first
	SPI1->CR1 &= ~(1U<<7);

	//Set frame size to 8 bits
	SPI1->CR1 &= ~(1U<<11);

	//Set clock frequency (f_periph/4)
	SPI1->CR1 |=(1U<<3);
	SPI1->CR1 &=~(1U<<4);
	SPI1->CR1 &=~(1U<<5);

	//Set NSS to software slave management
	SPI1->CR1 |= (1<<8);
	SPI1->CR1 |= (1<<9);

	//Set to full-duplex mode
	SPI1->CR1 &=~(1U<<10);

	//Set CPOL=1 & CPHA=1
	SPI1->CR1 |=(1U<<0);
	SPI1->CR1 |=(1U<<1);

	//Enable SPI1 module
	SPI1->CR1 |= (1<<6);

}

void spi1_write(uint8_t *data, uint32_t data_size){

	volatile uint32_t temp;

	uint32_t count = 0;

	while(data_size > count){

		//Wait till TXE=1
		while(!(SPI1->SR & TX_EMPTY));

		SPI1->DR = data[count];

		count++;
	}

	//Wait till TXE=1
	while(!(SPI1->SR & TX_EMPTY));

	//Wait until bus is free
	while(SPI1->SR & SPI_BUSY);

	//Clear OVR flag
	temp = SPI1->DR;
	temp = SPI1->SR;
}

void spi1_read(uint8_t *data, uint32_t data_size){

	while(data_size){

		SPI1->DR = 0;

		//Wait till RXNE is set
		while(!(SPI1->SR & (RX_NEMPTY))){}

		*data++ = SPI1->DR;

		data_size--;
	}
}

void cs_enable(void){

	GPIOA->ODR &=~(1U<<4);
}
void cs_disable(void){

	GPIOA->ODR |= (1U<<4);
}
