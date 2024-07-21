#include "stm32f4xx.h"

#define I2C1CLK_EN    (1U<<21)
#define GPIOBCLK_EN   (1U<<1)
#define I2C1EN        (1U<<0)
#define I2C_RST       (1U<<15)

#define SCL_FREQ      80
#define MAX_RISETIME  17

#define BUS_BUSY      (1U<<1)
#define I2C_START     (1U<<8)
#define START_SET     (1U<<0)
#define ADDR_SENT     (1U<<1)
#define DREG_EMPTY    (1U<<7)
#define ACK           (1U<<10)
#define I2C_STOP      (1U<<9)
#define DREG_NEMPTY   (1U<<6)
#define BTF           (1U<<2)

void i2c1_init(void) {

    // Enable clock for GPIOB
    RCC->AHB1ENR |= GPIOBCLK_EN;

    // Enable alternate function mode for PB8 and PB9
    GPIOB->MODER &= ~((1U<<16) | (1U<<18));
    GPIOB->MODER |= ((1U<<17) | (1U<<19));

    // Select alternate function mode for I2C (AF4) for PB8 and PB9
	GPIOB->AFR[1] &=~(1U<<0);
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |=(1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);

	GPIOB->AFR[1] &=~(1U<<4);
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |=(1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);

    // Set PB8 and PB9 output type to open-drain
    GPIOB->OTYPER |= (1U << 8) | (1U << 9);

    // Enable pull-up for PB8 and PB9
    GPIOB->PUPDR |= (1U << 16) | (1U << 18);
    GPIOB->PUPDR &= ~((1U << 17) | (1U << 19));

    // Enable clock for I2C1 module (APB1)
    RCC->APB1ENR |= I2C1CLK_EN;

    // Enter reset mode
    I2C1->CR1 |= I2C_RST;

    // Come out of reset
    I2C1->CR1 &= ~I2C_RST;

    // Set clock frequency
    I2C1->CR2 = (1U << 4);

    // Set I2C SCL frequency
    I2C1->CCR = SCL_FREQ;

    // Set max rise time
    I2C1->TRISE = MAX_RISETIME;

    // Enable I2C1
    I2C1->CR1 |= I2C1EN;

}

void i2c1_byte_read(char saddr, char mem_addr, char* data) {

    volatile int temp;

    // Wait until bus is not busy
    while (I2C1->SR2 & BUS_BUSY) {}

    // Generate start condition
    I2C1->CR1 |= I2C_START;

    // Wait till start flag is set
    while (!(I2C1->SR1 & START_SET)) {}

    // Send slave address with write
    I2C1->DR = saddr << 1;

    // Wait until address flag is set
    while (!(I2C1->SR1 & ADDR_SENT)) {}

    // Clear addr flag (by reading SR2)
    temp = I2C1->SR2;

    // Send register address of slave
    I2C1->DR = mem_addr;

    // Wait until transmitter empty
    while (!(I2C1->SR1 & DREG_EMPTY)) {}

    // Generate restart condition
    I2C1->CR1 |= I2C_START;

    // Wait till start flag is set
    while (!(I2C1->SR1 & START_SET)) {}

    // Send slave address with read
    I2C1->DR = saddr << 1 | 1;

    // Wait until address flag is set
    while (!(I2C1->SR1 & ADDR_SENT)) {}

    // Disable acknowledgment (Master sends a NACK)
    I2C1->CR1 &= ~ACK;

    // Clear address flag
    temp = I2C1->SR2;

    // Generate stop after data received
    I2C1->CR1 |= I2C_STOP;

    // Wait till RXNE flag is set
    while (!(I2C1->SR1 & DREG_NEMPTY)) {}

    // Read data from data register
    *data++ = I2C1->DR;

}

void i2c1_burst_read(char saddr, char mem_addr, int data_size, char* data) {
    volatile int temp;

    // Wait until bus is not busy
    while (I2C1->SR2 & BUS_BUSY) {}

    // Generate start condition
    I2C1->CR1 |= I2C_START;

    // Wait till start flag is set
    while (!(I2C1->SR1 & START_SET)) {}

    // Send slave address + Write
    I2C1->DR = saddr << 1;

    // Wait until address flag is set
    while (!(I2C1->SR1 & ADDR_SENT)) {}

    // Clear addr flag (by reading SR2)
    temp = I2C1->SR2;

    // Wait until transmitter empty
    while (!(I2C1->SR1 & DREG_EMPTY)) {}

    // Send register address of slave
    I2C1->DR = mem_addr;

    // Wait until transmitter empty
    while (!(I2C1->SR1 & DREG_EMPTY)) {}

    // Generate restart condition
    I2C1->CR1 |= I2C_START;

    // Wait till start flag is set
    while (!(I2C1->SR1 & START_SET)) {}

    // Send slave address with read
    I2C1->DR = saddr << 1 | 1;

    // Wait until address flag is set
    while (!(I2C1->SR1 & ADDR_SENT)) {}

    // Clear address flag
    temp = I2C1->SR2;

    // Enable acknowledgment
    I2C1->CR1 |= ACK;

    while (data_size > 0U) {
        if (data_size == 1U) {
            // Disable acknowledgment
            I2C1->CR1 &= ~ACK;

            // Generate stop
            I2C1->CR1 |= I2C_STOP;

            // Wait for RXNE flag set
            while (!(I2C1->SR1 & DREG_NEMPTY)) {}

            // Read data from data register
            *data++ = I2C1->DR;
            break;
        } else {
            // Wait until RXNE flag is set
            while (!(I2C1->SR1 & DREG_NEMPTY)) {}

            // Read data from data register
            *data++ = I2C1->DR;

            data_size--;
        }
    }
}

void i2c1_burst_write(char saddr, char mem_addr, int data_size, char* data) {
    volatile int temp;

    // Wait until bus is not busy
    while (I2C1->SR2 & BUS_BUSY) {}

    // Generate start condition
    I2C1->CR1 |= I2C_START;

    // Wait till start flag is set
    while (!(I2C1->SR1 & START_SET)) {}

    // Send slave address + Write
    I2C1->DR = saddr << 1;

    // Wait until address flag is set
    while (!(I2C1->SR1 & ADDR_SENT)) {}

    // Clear addr flag (by reading SR2)
    temp = I2C1->SR2;

    // Send register address of slave
    I2C1->DR = mem_addr;

    // Wait until transmitter empty
    while (!(I2C1->SR1 & DREG_EMPTY)) {}

    while (data_size > 0U) {
        // Wait until data register empty
        while (!(I2C1->SR1 & DREG_EMPTY)) {}

        // Send data
        I2C1->DR = *data++;

        data_size--;
    }

    // Wait until transfer finished
    while (!(I2C1->SR1 & BTF)) {}

    // Generate stop
    I2C1->CR1 |= I2C_STOP;
}
