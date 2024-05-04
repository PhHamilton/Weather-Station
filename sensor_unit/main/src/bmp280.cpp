#include "bmp280.h"
#include "i2c_handler.h"
#include <stdio.h>

#define N_COMPENSATION_BITS 24
#define N_TEMP_BITS 3
#define N_PRESS_BITS 3

uint8_t device_addr;
uint8_t buf[N_COMPENSATION_BITS];
BMP280_CONFIG BMP280_config;

typedef enum
{
	dig_T1_lsb_addr = 0x88,
	dig_P1_lsb_addr = 0x8E
}compensation_reg_lsb_addr_t;

typedef struct
{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;

}compensation_values_t;

compensation_values_t compensation_values;

void update_config_struct(BMP280_CONFIG* config);
BMP280_ERROR_CODES update_config();
BMP280_ERROR_CODES read_compensation_values();
float compute_temperature(int32_t t_fine);
float compute_pressure(uint32_t adc_press, int32_t t_fine);
int32_t compute_fine_temperature(uint32_t adc_temp);

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
	I2C_ERROR_CODES rc = read_reg(BMP280_config.device_addr, dig_T1_lsb_addr, buf, N_COMPENSATION_BITS);

	if(rc != I2C_OK)
		return BMP280_FAILED_TO_READ_DATA;

	compensation_values.dig_T1 = buf[1]  << 8 | buf[0];
	compensation_values.dig_T2 = buf[3]  << 8 | buf[2];
	compensation_values.dig_T3 = buf[5]  << 8 | buf[4];

	compensation_values.dig_P1 = buf[7]  << 8 | buf[6];
	compensation_values.dig_P2 = buf[9]  << 8 | buf[8];
	compensation_values.dig_P3 = buf[11] << 8 | buf[10];
	compensation_values.dig_P4 = buf[13] << 8 | buf[12];
	compensation_values.dig_P5 = buf[15] << 8 | buf[14];
	compensation_values.dig_P6 = buf[17] << 8 | buf[16];
	compensation_values.dig_P7 = buf[19] << 8 | buf[18];
	compensation_values.dig_P8 = buf[21] << 8 | buf[20];
	compensation_values.dig_P9 = buf[23] << 8 | buf[22];

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
		return BMP280_FAILED_TO_WRITE_DATA;
	return BMP280_OK;
}

void update_config_struct(BMP280_CONFIG* config)
{
	BMP280_config.device_addr = config->device_addr;
	BMP280_config.I2C_adapter = config->I2C_adapter;
	BMP280_config.mode = config->mode;
}

BMP280_ERROR_CODES get_BMP280_data(BMP280_DATA *data)
{
	update_config();

	I2C_ERROR_CODES rc = read_reg(BMP280_config.device_addr, PRESS_MSB, buf, N_PRESS_BITS + N_TEMP_BITS);

	if(rc != I2C_OK)
	{
		printf("failed.. %i\n", rc);
		return BMP280_FAILED_TO_READ_DATA;
	}
	uint32_t adc_press = buf[0] << 12 | buf[1] << 4 | buf[2] >> 4;
	uint32_t adc_temp  = buf[3] << 12 | buf[4] << 4 | buf[5] >> 4;

	int32_t t_fine = compute_fine_temperature(adc_temp);	
	
	data->temperature = compute_temperature(t_fine);
	data->pressure = compute_pressure(adc_press, t_fine);

	return BMP280_OK;
}

float compute_temperature(int32_t t_fine)
{
	return ((t_fine * 5 + 128) >> 8) * 0.01;
}

int32_t compute_fine_temperature(uint32_t adc_temp)
{
	int32_t var1, var2;

	var1 = ((((adc_temp>>3) - ((int32_t)compensation_values.dig_T1<<1))) 
			* ((int32_t)compensation_values.dig_T2)) >> 11;
	var2 = (((((adc_temp>>4) - ((int32_t)compensation_values.dig_T1)) 
			* ((adc_temp>>4) - ((int32_t)compensation_values.dig_T1))) >> 12) 
			* ((int32_t)compensation_values.dig_T3)) >> 14;

	return var1 + var2;
}

float compute_pressure(uint32_t adc_press, int32_t t_fine)
{
	int64_t var1, var2, p;
	var1 = ((int64_t) t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)compensation_values.dig_P6;
	var2 = var2 + ((var1 * (int64_t)compensation_values.dig_P5) << 17);
	var2 = var2 + (((int64_t)compensation_values.dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)compensation_values.dig_P3)>>8) + ((var1 * (int64_t)compensation_values.dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)compensation_values.dig_P1) >> 33;

	if(var1 == 0)
		return 0;
	p = 1048576 - adc_press;
	p = (((p << 31)-var2)*3125)/var1;
	var1 = (((int64_t)compensation_values.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)compensation_values.dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)compensation_values.dig_P7)<<4);
	

	return (uint32_t)p/(1000 * 256);
}
