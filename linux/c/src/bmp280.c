#include "bmp280.h"
#include "i2c_handler.h"
#include <stdio.h>

uint8_t device_addr;
uint8_t buf[8];
BMP280_CONFIG BMP280_config;

typedef enum
{
	dig_T1_lsb_addr = 0x88,
	dig_T1_msb_addr = 0x89,
	dig_T2_lsb_addr = 0x8A,
	dig_T2_msb_addr = 0x8B,
	dig_T3_lsb_addr = 0x8C,
	dig_T3_msb_addr = 0x8D
}compensation_reg_lsb_addr_t;

typedef struct
{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
}compensation_values_t;

compensation_values_t compensation_values;

void update_config_struct(BMP280_CONFIG* config);
BMP280_ERROR_CODES update_config();
BMP280_ERROR_CODES read_compensation_values();
float compute_temperature(uint32_t adc_temp);

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

	rc_BMP280 = read_compensation_values();
	if(rc_BMP280 != BMP280_OK)
		return BMP280_FAILED_TO_READ_DATA;

	return BMP280_OK;
}

BMP280_ERROR_CODES read_compensation_values()
{
	uint8_t buf[2];
	I2C_ERROR_CODES rc = read_reg(BMP280_config.device_addr, dig_T1_msb_addr, buf, 1);
	compensation_values.dig_T1 = buf[0] << 8;
	rc += read_reg(BMP280_config.device_addr, dig_T1_lsb_addr, buf, 1);
	compensation_values.dig_T1 |= buf[0];

	rc += read_reg(BMP280_config.device_addr, dig_T2_msb_addr, buf, 1);
	compensation_values.dig_T2 = buf[0] << 8;
	rc += read_reg(BMP280_config.device_addr, dig_T2_lsb_addr, buf, 1);
	compensation_values.dig_T2 |= buf[0];

	rc += read_reg(BMP280_config.device_addr, dig_T3_msb_addr, buf, 1);
	compensation_values.dig_T3 = buf[0] << 8;
	rc += read_reg(BMP280_config.device_addr, dig_T3_lsb_addr, buf, 1);
	compensation_values.dig_T3 |= buf[0];

	printf("Compensation values: T1 %i, T2 %i, T3 %i\n", compensation_values.dig_T1, compensation_values.dig_T2, compensation_values.dig_T3);
	if(rc != I2C_OK)
		return BMP280_FAILED_TO_READ_DATA;
	return BMP280_OK;
}

BMP280_ERROR_CODES update_config()
{
	uint8_t reg_settings = 1 << 5 | 1 << 2 | BMP280_config.mode;
	printf("%i\n", reg_settings);
	I2C_ERROR_CODES rc = write_to_reg(BMP280_config.device_addr, CTRL_MEAS, (uint8_t*)&reg_settings, 1);
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

BMP280_ERROR_CODES get_temperature(float *temperature)
{
	update_config();


	I2C_ERROR_CODES rc = read_reg(BMP280_config.device_addr, 0xFA, buf, 1);
	if(rc != I2C_OK)
		return BMP280_FAILED_TO_READ_DATA;

	uint32_t adc_temp = buf[0] << 12;
	rc = read_reg(BMP280_config.device_addr, 0xFB, buf, 1);
	if(rc != I2C_OK)
		return BMP280_FAILED_TO_READ_DATA;

	adc_temp |= buf[0] << 4;

	rc = read_reg(BMP280_config.device_addr, 0xFC, buf, 1);
	if(rc != I2C_OK)
		return BMP280_FAILED_TO_READ_DATA;

	adc_temp |= buf[0] >> 4;
	*temperature = compute_temperature(adc_temp);
	return BMP280_OK;
}

float compute_temperature(uint32_t adc_temp)
{
	int32_t var1, var2, temperature;

	var1 = ((((adc_temp>>3) - ((int32_t)compensation_values.dig_T1<<1))) 
			* ((int32_t)compensation_values.dig_T2)) >> 11;
	var2 = (((((adc_temp>>4) - ((int32_t)compensation_values.dig_T1)) 
			* ((adc_temp>>4) - ((int32_t)compensation_values.dig_T1))) >> 12) 
			* ((int32_t)compensation_values.dig_T3)) >> 14;
	
	temperature = ((var1 + var2) * 5 + 128) >> 8;

	printf("var1: %i, var2: %i, temp: %i\n", var1, var2, temperature);

	return temperature * 0.01;
}

BMP280_ERROR_CODES get_pressure()
{
	return BMP280_OK;
}
