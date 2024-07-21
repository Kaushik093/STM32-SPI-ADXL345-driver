#include <adxl.h>

char data;
uint8_t data_rec[6];

void adxl_init (void)
{
	//Enable I2C
	i2c1_init();

	//Read DEVID
	adxl_read_address(DEVID_R);

	//Set data format to +/-4g
	adxl_write (DATA_FORMAT_R, FOUR_G);

	//Reset all bits
	adxl_write (POWER_CTL_R, RESET);

	//Configure power control to set measure bit
	adxl_write (POWER_CTL_R, SET_MEASURE_B);
}

void adxl_read_values (uint8_t reg)
{
	i2c1_burst_read(DEVICE_ADDR, reg, 6,(char *)data_rec);

}


void adxl_read_address(uint8_t reg){

	i2c1_byte_read(DEVICE_ADDR,reg,&data);

}

void adxl_write (uint8_t reg, char value)
{
	char data[1];
	data[0] = value;

	i2c1_burst_write(DEVICE_ADDR, reg,1,data) ;
}
