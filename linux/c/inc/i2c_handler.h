#ifndef __I2C_HANDLER_H__
#define __I2C_HANDLER_H__

#include "file_handler.h"

typedef enum
{
	I2C_OK,
	FAILED_TO_OPEN_I2C_DEVICE,
	FAILED_TO_WRITE_TO_I2C_DEVICE,
	FAILED_TO_READ_REG,
	FAILED_TO_WRITE_REG,
	FAILED_TO_SET_DEVICE_ADDR
}I2C_ERROR_CODES;

I2C_ERROR_CODES initialize_i2c(const char* I2CAdapter);
I2C_ERROR_CODES write_to_reg(uint8_t device_addr, uint8_t reg, uint8_t data[], uint8_t data_size);
I2C_ERROR_CODES read_reg(uint8_t device_addr, uint8_t reg, uint8_t buf[], uint8_t buf_size);

#endif //__I2C_HANDLER_H__
