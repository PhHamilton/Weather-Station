#include "i2c_handler.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_PATH "/dev"

char I2C_device[BUFF_SIZE];
uint8_t I2C_file;
I2C_ERROR_CODES rc;

I2C_ERROR_CODES open_i2c_device();
I2C_ERROR_CODES set_device_addr(uint8_t device_addr);

I2C_ERROR_CODES initialize_i2c(const char* I2CAdapter)
{
	sprintf(I2C_device, "%s/%s", I2C_PATH, I2CAdapter);	
	rc = open_i2c_device();	
	if(rc != I2C_OK) return rc;

	return I2C_OK;
}

I2C_ERROR_CODES write_to_reg(uint8_t device_addr, uint8_t reg, uint8_t data[], uint8_t data_size)
{
	rc = set_device_addr(device_addr);
	if(rc != I2C_OK) return rc;

	if(write(I2C_file, data, data_size) != data_size)
	{
		return FAILED_TO_WRITE_TO_I2C_DEVICE;
	}

	return I2C_OK;	

}

I2C_ERROR_CODES read_reg(uint8_t device_addr, uint8_t reg, uint8_t buf[], uint8_t buf_size)
{
	rc = set_device_addr(device_addr);
	if(rc != I2C_OK) return rc;

	if(read(I2C_file, buf, buf_size) != buf_size)
		return FAILED_TO_READ_REG;

	return I2C_OK;	
}

I2C_ERROR_CODES set_device_addr(uint8_t device_addr)
{
	if(ioctl(I2C_file, I2C_SLAVE, device_addr))
		return FAILED_TO_SET_DEVICE_ADDR;

	return I2C_OK;
}

I2C_ERROR_CODES open_i2c_device()
{
	if(open(I2C_device, O_RDWR) < 0)
		return FAILED_TO_OPEN_I2C_DEVICE;

	return I2C_OK;
}

I2C_ERROR_CODES close_i2c_device()
{
	close(I2C_file);
	return I2C_OK;
}
