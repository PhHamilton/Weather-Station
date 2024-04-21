#ifndef __BMP280_H__
#define __BMP280_H__
#include <stdint.h>

#define BMP280_ID 0x58
typedef enum
{
	ID = 0xD0,
	RESET = 0xE0,
	STATUS = 0xF3,
	CTRL_MEAS = 0xF4
}BMP280_REGISTERS;

typedef enum
{
	SKIPPED      	 = 0x00,
	OVERSAMPLING_X1  = 0x01,
	OVERSAMPLING_X2  = 0x02,
	OVERSAMPLING_X4  = 0x03,
	OVERSAMPLING_X8  = 0x04,
	OVERSAMPLING_X16 = 0x05
}BMP280_OSRS;

typedef enum
{
	SLEEP  = 0x00,
	FORCED = 0x01,
	NORMAL = 0x03
}BMP280_MODES;

typedef enum
{
	BMP280_OK,
	BMP280_UNKNOWN_DEVICE,
	BMP280_FAILED_TO_CONFIGURE_DATA,
	BMP280_FAILED_TO_READ_DATA
}BMP280_ERROR_CODES;

typedef struct
{
	uint8_t device_addr;
	const char* I2C_adapter;
	BMP280_MODES mode;
}BMP280_CONFIG;

BMP280_ERROR_CODES initialize_BMP280(BMP280_CONFIG* config);
BMP280_ERROR_CODES get_temperature(float *temperature);
BMP280_ERROR_CODES get_pressure();

#endif //__BMP280_H__
