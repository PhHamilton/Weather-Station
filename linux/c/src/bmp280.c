#include "bmp280.h"
#include "i2c_handler.h"

uint8_t device_addr;
uint8_t buf[8];
BMP280_CONFIG BMP280_config;

void update_config_struct(BMP280_CONFIG* config);
BMP280_ERROR_CODES update_config();

BMP280_ERROR_CODES initialize_BMP280(BMP280_CONFIG* config)
{
	update_config_struct(config);
	I2C_ERROR_CODES rc = initialize_i2c(BMP280_config.I2C_adapter);
	if(rc != I2C_OK)
		return rc;
	rc = read_reg(BMP280_config.device_addr, ID, buf, 1);

	if(rc != I2C_OK || buf[0] != BMP280_ID)
		return BMP280_FAILED_TO_READ_DATA;
	
	BMP280_ERROR_CODES rc_BMP280 = update_config();
	if(rc_BMP280 != BMP280_OK)
		return rc_BMP280;

	return BMP280_OK;
}

BMP280_ERROR_CODES update_config()
{
	I2C_ERROR_CODES rc = write_to_reg(BMP280_config.device_addr, CTRL_MEAS, (uint8_t*)&BMP280_config.mode, 1);
	if(rc != I2C_OK)
		return BMP280_FAILED_TO_READ_DATA;
	return BMP280_OK;
}

void update_config_struct(BMP280_CONFIG* config)
{
	BMP280_config.device_addr = config->device_addr;
	BMP280_config.I2C_adapter = config->I2C_adapter;
	BMP280_config.mode = config->mode;
}

BMP280_ERROR_CODES get_temperature()
{
	
	return BMP280_OK;
}

BMP280_ERROR_CODES get_pressure()
{
	return BMP280_OK;
}
