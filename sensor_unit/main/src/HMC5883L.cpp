#include "HMC5883L.h"
#include <math.h>

#define N_ID_REG   0x03
#define MODE_MSK   0x03
#define N_AXIS_REG 0x06
static uint8_t buf[12] = {0};

typedef enum
{
    CONFIG_REG_A = 0x00,
    CONFIG_REG_B = 0x01,
    MODE_REG     = 0x02,
    X_MSB_REG    = 0x03,
    X_LSB_REG    = 0x04,
    Z_MSB_REG    = 0x05,
    Z_LSB_REG    = 0x06,
    Y_MSB_REG    = 0x07,
    Y_LSB_REG    = 0x08,
    STATUS_REG   = 0x09,
    ID_A_REG     = 0x0A,
    ID_B_REG     = 0x0B,
    ID_C_REG     = 0x0C
}HMC5883L_REGISTERS;

HMC5883L::HMC5883L(uint8_t I2CAddress) : _I2CHandler(I2CAddress)
{
    Serial.println("Initializing I2C..");
    _I2CHandler.Initialize();
}

HMC5883L_ERROR_CODES HMC5883L::Initialize(HMC5883L_CONFIG_t *config)
{
    I2C_ERROR_CODES rc = _I2CHandler.ReadFromReg(ID_A_REG, buf, N_ID_REG);
    if(rc != I2C_OK)
    {
        return HMC5883L_FAILED_TO_READ_DATA;
    }

    if(buf[0] != 'H' || buf[1] != '4' || buf[2] != '3')
    {
        return HMC5883L_UNKNOWN_DEVICE;
    }

    HMC5883L_ERROR_CODES rcHMC5883L = UpdateConfig(config);

    return rcHMC5883L == HMC5883L_OK ? HMC5883L_OK : HMC5883L_FAILED_TO_READ_DATA;
}

HMC5883L_ERROR_CODES HMC5883L::UpdateConfig(HMC5883L_CONFIG_t *config)
{
    buf[0] =    (uint8_t) (config->confA.nAverages << 5)
             || (uint8_t) (config->confA.outputDataRate << 2)
             || (uint8_t) (config->confA.mode);

    uint8_t rc = _I2CHandler.WriteToReg(CONFIG_REG_A, buf, 1);

    buf[0] = (uint8_t) (config->confB.gain << 5);
    rc    += _I2CHandler.WriteToReg(CONFIG_REG_B, buf, 1);

    return rc == I2C_OK ? HMC5883L_OK : HMC5883L_FAILED_TO_READ_DATA;
}

HMC5883L_ERROR_CODES HMC5883L::ChangeMode(HMC5883L_OPERATING_MODES_t mode)
{
    buf[0] = mode;
    I2C_ERROR_CODES rc = _I2CHandler.WriteToReg(MODE_REG, buf, 1);

    if(rc != I2C_OK)
        return HMC5883L_FAILED_TO_WRITE_DATA;

    return HMC5883L_OK;
}

float HMC5883L::GetHeading(void)
{
    HMC5883L_AXES_t axes = {.x = 0, .y = 0, .z = 0};
    _readAxisData(&axes);
    //Prevent division by zero
    if(axes.x == 0)
    {
      axes.x = 1;
    }
    float heading = atan2(axes.y, axes.x) * 180 / M_PI;

    if(heading < 0)
    {
        heading += 360;
    }
    return heading;
}

HMC5883L_ERROR_CODES HMC5883L::GetRawMeasurements(HMC5883L_AXES_t *axes)
{
    return _readAxisData(axes);
}

HMC5883L_ERROR_CODES HMC5883L::_readAxisData(HMC5883L_AXES_t *axes)
{
    HMC5883L_ERROR_CODES rcHMC5883L = ChangeMode(HMC5883L_OPERATING_MODES_t::SINGLE_MEASUREMENT);
    if(rcHMC5883L != HMC5883L_OK)
      return HMC5773L_FAILED_TO_CHANGE_MODE;

    I2C_ERROR_CODES rc = _I2CHandler.ReadFromReg(X_MSB_REG, buf, N_AXIS_REG);
    if(rc != I2C_OK)
        return HMC5883L_FAILED_TO_READ_DATA;

    axes->x = (int16_t) (buf[0] << 8 | buf[1]);
    axes->z = (int16_t) (buf[2] << 8 | buf[3]);
    axes->y = (int16_t) (buf[4] << 8 | buf[5]);

    /*
    axes->x = (int16_t) (buf[1] << 8 | buf[0]);
    axes->z = (int16_t) (buf[3] << 8 | buf[2]);
    axes->y = (int16_t) (buf[5] << 8 | buf[4]);
    */
    return HMC5883L_OK;
}

uint16_t HMC5883L::_getGainScaleFactor(HMC5883L_GAIN_t gain)
{
    uint16_t scaleFactor = 0;
    switch(gain)
    {
        case GAIN_0_88:
            scaleFactor = 1370;
        break;
        case GAIN_1_3:
            scaleFactor = 1090;
        break;
        case GAIN_1_9:
            scaleFactor = 820;
        break;
        case GAIN_2_5:
            scaleFactor = 660;
        break;
        case GAIN_4_0:
            scaleFactor = 440;
        break;
        case GAIN_4_7:
            scaleFactor = 390;
        break;
        case GAIN_5_6:
            scaleFactor = 330;
        break;
        case GAIN_8_1:
            scaleFactor = 230;
        break;
        default:
            scaleFactor = 1;
        break;
    }

    return scaleFactor;
}
