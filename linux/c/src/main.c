#include <stdint.h>
#include "i2c_handler.h"

#define BMP280_ADDR 76

int main(int argc, char** argv)
{
	char *I2C_adapter = "i2c-1";
	if(initialize_i2c(I2C_adapter) != I2C_OK)
	{
		printf("Failed to initialize I2C\n");
	}

	uint8_t data_buf;
	uint8_t rc = read_reg(BMP280_ADDR, 0xD0, (uint8_t *)&data_buf, 1);

	if(rc != I2C_OK)
		printf("Failed to read reg: %i\n", rc);
	else
		printf("%i\n", data_buf);

	return 0;
}
