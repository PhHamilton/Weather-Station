#include "../inc/i2cHandler.h"
#include <Wire.h>

I2CHandler::I2CHandler(uint16_t deviceAddress)
{
    _deviceAddress = deviceAddress;
}

I2C_ERROR_CODES I2CHandler::Initialize()
{
    Wire.begin();

    return I2C_ERROR_CODES::I2C_OK;
}

I2C_ERROR_CODES I2CHandler::WriteToReg(uint8_t reg, uint8_t data[], uint8_t dataSize)
{
    Wire.beginTransmission(_deviceAddress);
    Wire.write(reg);
    Wire.write(data, dataSize);
    Wire.endTransmission();
    return I2C_ERROR_CODES::I2C_OK;
}

I2C_ERROR_CODES I2CHandler::ReadFromReg(uint8_t reg, uint8_t buf[], uint8_t bufSize)
{
    Wire.beginTransmission(_deviceAddress);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_deviceAddress, bufSize);

    unsigned long startTime = millis();

    while(Wire.available() < bufSize)
    {
        if(millis() - startTime > 5000)
        {
            return FAILED_TO_READ_REG;
        }
    }


    uint8_t i = 0;
    while(Wire.available() && i < bufSize)
    {
        buf[i++] = Wire.read();
    }

    return I2C_ERROR_CODES::I2C_OK;
}
