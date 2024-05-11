#ifndef __HMC5883L.h__
#define __HMC5883L.h__

typedef struct
{
    HMC5883L_CONFIG_REG_A confA;
    HMC5883L_CONFIG_REG_B confB;
}HMC5883L_CONFIG_t;

typedef struct
{
    HMC5883L_AVG_SAMPLES_t nAverages;
    HMC5883L_ODR_t outputDataRate;
    HMC5883L_MEASUREMENT_MODE_t mode;
}HMC5883L_CONFIG_REG_A;

typedef struct
{
    HMC5883L_GAIN_t gain;
}CONFIG_REG_B;

typedef enum
{
    AVG_1 = 0x00,
    AVG_2 = 0x01,
    AVG_4 = 0x02,
    AVG_8 = 0x03,
}HMC5883L_AVG_SAMPLES_t;

typedef enum
{
    ODR_0_75 = 0x00,
    ODR_1_5  = 0x01,
    ODR_3    = 0x02,
    ODR_7_5  = 0x03,
    ODR_15   = 0x04,
    ODR_30   = 0x05,
    ODR_75   = 0x06
}HMC5883L_ODR_t;

typedef enum
{
    NORMAL        = 0x00,
    POSITIVE_BIAS = 0x01,
    NEGATIVE_BIAS = 0x02
}HMC5883L_MEASUREMENT_MODE_t;

typedef enum
{
    GAIN_0_88 = 0x00,
    GAIN_1_3  = 0x01,
    GAIN_1_9  = 0x02,
    GAIN_2_5  = 0x03,
    GAIN_4_0  = 0x04,
    GAIN_4_7  = 0x05,
    GAIN_5_6  = 0x06,
    GAIN_8_1  = 0x07
}HMC5883L_GAIN_t;

typedef enum
{
    CONTINOUS          = 0x00,
    SINGLE_MEASUREMENT = 0x01,
    IDLE               = 0x02
}HMC5883L_OPERATING_MODES_t;

typedef struct
{
    int16_t x,
    int16_t y,
    int16_t z
}HMC5883L_AXES_t;

typedef enum
{
    HMC5883L_OK
}HMC5883L_ERROR_CODES;

class HMC588L
{
    public:
        HMC88L(uint8_t deviceAddress);
        HMC5883L_ERROR_CODES Initialize();
        HMC5883L_ERROR_CODES ChangeSettings(HMC5883L_CONFIG_t *config);
        HMC5883L_ERROR_CODES ChangeMode(HMC5883L_OPERATING_MODES_t *mode);
        HMC5883L_ERROR_CODES GetRawMeasurements(HMC5883L_AXES_t *axes);
        float GetHeading(void);
}
#endif //__HMC5883L.h__
