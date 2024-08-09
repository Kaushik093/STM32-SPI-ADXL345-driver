=======
# STM32-ADXL345-SPI-Driver

## Table of contents
1. [Objective](#objective)
2. [Hardware](#hardware)
  1. [SPI-MCU pin mapping](#spi-mcu-pin-mapping)
     

## Objective 
Design an API that can be used to configure an ADXL345 Accelerometer and read data from it using the SPI protocol.

## Hardware
1. **MCU (Microcontroller unit)**: STM32F401RE Nucleo development board.
2. **Peripheral**: ADXL345 Accelerometer.

### SPI-MCU pin mapping 
| SPI Pin       | MCU GPIO Pin      |
| ------------- | ------------- |
| NSS           | PA4 (A2)  |
| SCK          | PA5 (D13)  |
| MISO          | PA6 (D12)  |
| MOSI          | PA7 (D11)  |

### ADXL345-MCU Connections
| ADXL345       | MCU GPIO Pin      |
| ------------- | ------------- |
| CS            | PA4 (A2)  |
| SCL          | PA5 (D13)  |
| SDO          | PA6 (D12)  |
| SDA          | PA7 (D11)  |
| GND          | PA6 (D12)  |
| VCC          | PA7 (D11)  |

## SPI API Design

SPI Module on STM32 used: SPI1

Alternate function for GPIO: AF05

### SPI1 initialization 

1. Enable clock access to GPIO A.
2. Set pins to alternate function mode and set the alternate function to AF05.
3. Enable Clock access for SPI1 module. (APB2 bus).
4. Set it to master.
5. Set data size.
6. Set *CPOL = 1 and CPHA = 1* (Refer to peripheral datasheet for this configuration).
7. Set MSB first. 
8. Set clock frequency (freq_periph/4).
9. Set NSS to software slave management. 
10. Enable SPI1.

### Write function
Arguments
1. Data array.
2. Data size (Number of bytes).

#### Code design
1. Check if the bus is busy.
2. Ensure that TXE is set to 1.
3. Write the first data item to transmit to the DR register. 
4. Wait until TXE=1 and write the second data item to be transmitted. 
5. After all data items are written ensure that TXE=1, Bus is free and clear OVR flag.

The software must ensure that the TXE flag is set to 1 before attempting to write to the Tx buffer. Otherwise, it overwrites the data previously written to the Tx buffer.

### Read function
#### Arguments
1. Data array.
2. Data size (Number of bytes).

#### Code design
1. Wait until RXNE=1 and read DR to get data.

## ADXL345 API Design

### Initializing ADXL345 

1. Initialize SPI1.
2. Set data format to +/- 4g.
3. Reset all bits.
4. Set ADXL to measuring mode.


### Writing to ADXL345

#### Function arguments 
1. Memory address
2. Data

#### Code design:
1. Set the multiple-byte bit, located after the R/W bit in the first byte transfer for multi-byte read/write in a single transmission, (Refer to Figure 37 in ADXL345 datasheet ).
2. Pull CS low.
3. Call SPI write function.
4. Pull CS high.

### Reading from ADXL345

#### Arguments:
1. Memory address.
2. Data.

#### Code design:
1. Set R-bit and multiple byte bits.
2. Pull CS low.
3. Send address.
4. Read data (6 bytes).
5. Pull CS high.

## ADXL345-SPI API Interaction Sequence
![I2C_Sequence](https://github.com/user-attachments/assets/309dea9d-4dad-4847-bcaf-a9be3405ab2c)
