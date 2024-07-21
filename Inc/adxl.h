#ifndef ADXL_H_
#define ADXL_H_

#include <i2c.h>
#include <stdint.h>

#define   DEVID_R					(0x00)
#define   DEVICE_ADDR    			(0x53)
#define   DATA_FORMAT_R   			(0x31)
#define   POWER_CTL_R 				(0x2D)
#define   DATA_START_ADDR			(0x32)
#define   DATA_FORMAT_R   			(0x31)


#define		FOUR_G					(0x01)
#define		RESET					(0x00)
#define     SET_MEASURE_B		    (0x08)

void adxl_init (void);
void adxl_read_values (uint8_t reg);
void adxl_read_address(uint8_t reg);
void adxl_write (uint8_t reg, char value);


#endif /* ADXL_H_ */
