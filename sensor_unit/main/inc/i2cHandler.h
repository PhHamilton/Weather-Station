#ifndef __I2CHANDLER_H__
#define __I2CHANDLER_H__

#include "Arduino.h"
#include <Wire.h>

#define I2C_ADAPTER_BUF_SIZE 50
#define I2C_BUF_SIZE 255

typedef enum
{
	I2C_OK,
	FAILED_TO_READ_REG,
	FAILED_TO_WRITE_REG,
}I2C_ERROR_CODES;

class I2CHandler
{
    public:
        I2CHandler(uint8_t deviceAddress);
        I2C_ERROR_CODES Initialize();
        I2C_ERROR_CODES WriteToReg(uint8_t reg, uint8_t data[], uint8_t dataSize);
        I2C_ERROR_CODES ReadFromReg(uint8_t reg, uint8_t buf[], uint8_t bufSize);
    private:
        uint8_t _deviceAddress;

};

#endif //__I2C_HANDLER_H__
