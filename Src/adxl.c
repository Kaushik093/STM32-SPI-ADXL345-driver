#include <adxl.h>
#include <spi.h>

#define   MULTI_BYTE_EN				0x40  //01000000
#define	  READ_OPERATION		    0x80

void adxl_init(){

	//Initialize SPI1 GPIO
	spi1_gpio_init();

	//Initialize SPI1
	spi1_init();

	/*Configure ADXL345*/

	//Set data format range to +-4g//
	adxl_write (DATA_FORMAT_R, FOUR_G);

//	//Reset all bits//
	adxl_write (POWER_CTL_R, RESET);

	//Configure power control measure bit//
	adxl_write (POWER_CTL_R, SET_MEASURE_B);
}

void adxl_read(uint8_t memAddr, uint8_t *data)
{

	//Set R-bit & multi-byte
	memAddr |= READ_OPERATION;

	memAddr |= MULTI_BYTE_EN;

	//Pull CS low
	cs_enable();

	//Send memory address
	spi1_write(&memAddr,1);

	//Call spi1_read()
	spi1_read(data,6);

	//Pull CS high
	cs_disable();

}

void adxl_write(uint8_t memAddr, uint8_t data)
{
	uint8_t data_array[2];

	data_array[0] = memAddr|MULTI_BYTE_EN;
	data_array[1] = data;

	//Pull CS low
	cs_enable();

	//Call Write function
	spi1_write(data_array,2);

	//Pull CS high
	cs_disable();

}
