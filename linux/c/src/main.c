#include <stdio.h>
#include <stdint.h>
#include "bmp280.h"
#include "i2c_handler.h"

#define BMP280_ADDR 0x76

int main(int argc, char** argv)
{
	BMP280_CONFIG cfg = {.device_addr = BMP280_ADDR, .I2C_adapter = "i2c-1", .mode = FORCED};

	if(initialize_BMP280(&cfg) != BMP280_OK)
		printf("Failed to initialize BMP280\n");
	else
		printf("BMP280 initialized successfully!\n");

	uint8_t data_buf;
	uint8_t rc = read_reg(BMP280_ADDR, 0xD0, (uint8_t *)&data_buf, 1);

	if(rc != I2C_OK)
		printf("Failed to read reg: %i\n", rc);
	else
		printf("Successfully read register: %i\n", data_buf);

	rc = read_reg(BMP280_ADDR, 0xF4, (uint8_t *)&data_buf, 1);

	if(rc != I2C_OK)
		printf("Failed to read reg: %i\n", rc);
	else
		printf("Successfully read register: %i\n", data_buf);

	BMP280_DATA data;
	if(get_BMP280_data(&data) != BMP280_OK)
		printf("Failed to read data!\n"); 	
	else
		printf("Temperature: %f, Pressure: %f\n", data.temperature, data.pressure); 	

	return 0;
}
