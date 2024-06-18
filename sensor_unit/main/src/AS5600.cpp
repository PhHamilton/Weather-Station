#include "AS5600.h"

uint8_t buf[8];

typedef enum
{
    STATUS_REG    = 0x0B,
    RAW_ANGLE_REG = 0x0C
}AS5600_REG_t;

AS5600::AS5600(uint8_t I2CAddress) : _I2CHandler(I2CAddress)
{
}

bool AS5600::Initialize()
{
  I2C_ERROR_CODES rc = _I2CHandler.Initialize();

  return (rc == I2C_OK) ? true : false;
}
uint16_t AS5600::ReadRawAngle(void)
{
  _I2CHandler.ReadFromReg(RAW_ANGLE_REG, buf, 2);
  return (buf[0] << 8) | buf[1];
}

AS5600_STATUS_t AS5600::CheckMagnet(void)
{
  _I2CHandler.ReadFromReg(STATUS_REG, buf, 1);

  uint8_t status = buf[0] >> 3;
  switch(status)
  {
    case 4:
        return MAGNET_OK;
    case 5:
        return MAGNET_TO_STRONG;
    case 6:
        return MAGNET_TO_WEAK;
    default:
        return UNKNOWN_STATE;
  };
}
