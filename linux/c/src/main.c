#include <stdio.h>
#include <stdint.h>
#include "bmp280.h"

#define BMP280_ADDR 0x76

int main(int argc, char** argv)
{
	BMP280_CONFIG cfg = {.device_addr = BMP280_ADDR, .I2C_adapter = "i2c-1", .mode = FORCED};

	if(initialize_BMP280(&cfg) != BMP280_OK)
		printf("Failed to initialize BMP280\n");
	else
		printf("BMP280 initialized successfully!\n");
/*
	if(initialize_i2c(I2C_adapter) != I2C_OK)
	{
		printf("Failed to initialize I2C\n");
	}
	else
		printf("I2C initialized successfully!\n");

	uint8_t data_buf;
	uint8_t rc = read_reg(BMP280_ADDR, 0xD0, (uint8_t *)&data_buf, 1);

	if(rc != I2C_OK)
		printf("Failed to read reg: %i\n", rc);
	else
		printf("Successfully read register: %i\n", data_buf);
*/

	return 0;
}
