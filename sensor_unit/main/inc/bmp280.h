#ifndef __BMP280_H__
#define __BMP280_H__
#include <stdint.h>
#include "../inc/i2cHandler.h"

#define BMP280_ID 0x58

typedef enum
{
    SKIPPED          = 0x00,
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
    BMP280_FAILED_TO_READ_DATA,
    BMP280_FAILED_TO_WRITE_DATA
}BMP280_ERROR_CODES;

typedef struct
{
    BMP280_MODES mode;
    BMP280_OSRS overSamplingTemp;
    BMP280_OSRS overSamplingPress;
}BMP280_CONFIG;

class BMP280
{
    public:
        BMP280(uint8_t I2CAddress);
        BMP280_ERROR_CODES Initialize(BMP280_CONFIG *config);
        BMP280_ERROR_CODES ChangeMode(BMP280_MODES mode);
        BMP280_ERROR_CODES UpdateConfig(BMP280_CONFIG *config);
        BMP280_ERROR_CODES ReadData(void);
        float GetTemperature(void);
        float GetPressure(void);

    private:
        float _temperature;
        float _pressure;
        BMP280_CONFIG _config;

        float _computeTemperature(int32_t t_fine);
        float _computePressure(uint32_t adc_press, int32_t t_fine);
        int32_t _computeFineTemperature(uint32_t adc_temp);
        BMP280_ERROR_CODES _readCompensationValues();
        I2CHandler _I2CHandler;

};

#endif //__BMP280_H__
