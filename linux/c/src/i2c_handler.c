#include "i2c_handler.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

#define I2C_PATH "/dev"

char _I2CAdapter[I2C_ADAPTER_BUF_SIZE];
char buffer[I2C_BUF_SIZE];
uint8_t I2C_file;
uint16_t file;

I2C_ERROR_CODES rc;

I2C_ERROR_CODES open_i2c_bus();
I2C_ERROR_CODES set_device_addr(uint8_t device_addr);

I2C_ERROR_CODES initialize_i2c(const char* I2CAdapter)
{
	sprintf(_I2CAdapter, "%s/%s", I2C_PATH, I2CAdapter);	
	return I2C_OK;
}

I2C_ERROR_CODES read_reg(uint8_t device_addr, uint8_t reg, uint8_t buf[], uint8_t buf_size)
{
	rc = open_i2c_bus(_I2CAdapter);	

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
	memset(buffer, 0, I2C_BUF_SIZE);
	open_i2c_bus(_I2CAdapter);

	if (ioctl(file, I2C_SLAVE, device_addr) < 0)
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		return FAILED_TO_READ_REG;
	}	
	buffer[0] = reg;
	for(uint8_t i = 0; i < data_size; i++)
		buffer[i+1] = data[i];

	uint8_t formatted_size = data_size + 1; // Add 1 for reg addr
	if (write(file, buffer, formatted_size) != formatted_size)
	{
		return FAILED_TO_WRITE_TO_I2C_DEVICE;
	}
	return I2C_OK;
}

I2C_ERROR_CODES open_i2c_bus(const char* I2CAdapter)
{
	file = open(I2CAdapter, O_RDWR);
	if(file < 0)
	{
		return FAILED_TO_OPEN_I2C_DEVICE;
	}

	return I2C_OK;
}
