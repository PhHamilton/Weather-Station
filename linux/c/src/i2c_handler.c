#include "i2c_handler.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_PATH "/dev"

char buffer[2];
uint8_t I2C_file;

I2C_ERROR_CODES rc;

I2C_ERROR_CODES open_i2c_bus();
I2C_ERROR_CODES set_device_addr(uint8_t device_addr);

I2C_ERROR_CODES initialize_i2c(const char* I2CAdapter)
{
	sprintf(buffer, "%s/%s", I2C_PATH, I2CAdapter);	
	rc = open_i2c_bus();	
	if(rc != I2C_OK) return rc;

	return I2C_OK;
}

I2C_ERROR_CODES read_reg(uint8_t device_addr, uint8_t reg, uint8_t buf[], uint8_t buf_size)
{
	int file;
	const char* filename = "/dev/i2c-1";
	file = open(filename, O_RDWR);
	if (file < 0)
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		return FAILED_TO_OPEN_I2C_DEVICE;
	}

	if (ioctl(file, I2C_SLAVE, device_addr) < 0)
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		return FAILED_TO_READ_REG;
	}	
	buf[0] = reg;

	if (write(file, buf, 1) != 1)
	{
		/* ERROR HANDLING: i2c transaction failed */
		return FAILED_TO_WRITE_TO_I2C_DEVICE;
	}

	/* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
	if (read(file, buf, 1) != 1)
	{
		return FAILED_TO_READ_REG;
	}

	return I2C_OK;
}

I2C_ERROR_CODES write_to_reg(uint8_t device_addr, uint8_t reg, uint8_t data[], uint8_t data_size)
{
	return I2C_OK;
}

I2C_ERROR_CODES open_i2c_bus(const char* I2CAdapter)
{
	if((I2C_file = open(I2CAdapter, O_RDWR)) < 0)
	{
		return FAILED_TO_OPEN_I2C_DEVICE;
	}

	return I2C_OK;
}
