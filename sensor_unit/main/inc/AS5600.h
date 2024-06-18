#ifndef __AS5600_H__
#define __AS5600_H__

#include <stdint.h>
#include "i2cHandler.h"

#define AS5600_ADDR 0x36

typedef enum
{
    MAGNET_TO_STRONG,
    MAGNET_TO_WEAK,
    MAGNET_OK,
    UNKNOWN_STATE
}AS5600_STATUS_t;

class AS5600
{
    public:
        AS5600(uint8_t I2CAddress);
        bool Initialize();
        uint16_t ReadRawAngle(void);
        AS5600_STATUS_t CheckMagnet(void);
    private:
        uint16_t _rawAngle;
        I2CHandler _I2CHandler;
};
#endif
