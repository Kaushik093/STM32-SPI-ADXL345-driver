
# STM32-UART-Drivers

## Table of Contents

1. [Overview](#overview)
2. [Hardware Requirements](#hardware-requirements)
3. [Software Requirements](#software-requirements)
4. [Developing and testing the transmitter driver](#developing-and-testing-the-transmitter-driver)
5. [Developing and testing the receiver driver](#developing-and-testing-the-receiver-driver)

## Overview
This project aims to develop drivers for the UART protocol using bare-metal C code for transmitting and receiving data between an STM32 microcontroller and a personal computer.

## Hardware requirements
1. STM32F4 microcontroller (e.g., STM32F401RE Nucleo board)
2. USB-to-serial adapter (if not using a development board with built-in USB-to-UART functionality)
3. Personal computer

## Software Requirements
1. STM32Cube IDE
2. STM32CubeF4 package (For CMSIS libraries)
3. Serial terminal application (e.g. RealTerm)

## Developing and testing the transmitter driver
The USART2 module on the STM32 microcontroller will be used for this project as it is connected to USB. The base STM32 UART pins are connected to the USB pins with a built-in UART to USB bridge, allowing direct communication with the USB serial to read on a COM port.

### USART2 Configuration (Needed??)
USART2 Clock bus: APB1 (Refer to the STM32F401xB/STM32F401xC block diagram in the datasheet)
#### UART pins:
1. USART2_TX: PA2
2. USART2_RX: PA3  
*Pins for USART2 can be found in the alternate function mapping table in the datasheet.*

#### GPIOA Pin register configuration:
1. Enable clock access to GPIO A.
2. Enable alternate function mode on PA2.
3. Set PA2 alternate function type to UART_TX (AF07 (Refer to the alternate function mapping table in the datasheet)).

#### USART register configuration:
1. Enable clock on the APB1 bus for USART2
2. Define a function to calculate the baud-rate divisor and set the returned value to the baud rate register. 
3. Configure transfer direction (Enable transmitter).
4. Enable the UART module.

#### Transmitting data:
Ensure that the transmit data register is empty and then write to the data register. 

#### Testing the transmitter:
Download the Realterm software onto your desktop. Realterm prints the data received on the USB port. Configure the baud rate and the port in the Port tab on the Realterm window.  
To print multiple lines call the printf function with the desired data to be printed and reimplement the putchar function.

In embedded systems, especially when using UART for communication, you often need to define your putchar function to redirect printf output to the UART. This is because the standard library's putchar function is designed to output characters to the console, not through UART.

## Developing and testing the receiver driver
