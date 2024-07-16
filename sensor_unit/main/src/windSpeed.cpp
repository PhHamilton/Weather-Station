#include "../inc/windSensor.h"

WindSensor::WindSensor(uint8_t HMC5883L_Addr, uint8_t AS5600_Addr) :
    directionSensor(HMC5883L_Addr), speedSensor(AS5600_Addr)
{

}

bool WindSensor::Initialize()
{
    if(directionSensor.Initialize(&config) != HMC5883L_OK)
        return false;
    if(!speedSensor.Initialize())
        return false;
    return true;
}

float WindSensor::GetWindSpeed()
{
    prevSpeed = speedSensor.ReadRawAngle();
    uint8_t counter = 0;
    while(counter < SPEED_MEAS_FREQ_HZ)
    {
        currentMillis = millis();
        if(currentMillis - previousMillis >= 1/SPEED_MEAS_FREQ_HZ * 1000)
        {
            uint16_t currentSpeed = speedSensor.ReadRawAngle();
            buf[counter] = (currentSpeed - prevSpeed) / (1000/SPEED_MEAS_FREQ_HZ);
            prevSpeed = currentSpeed;
            previousMillis = currentMillis;
            counter++;
        }
    }
    float speed = 0;
    for(uint8_t i = 0; i < SPEED_MEAS_FREQ_HZ; i++)
        speed += buf[i];
    return speed/SPEED_MEAS_FREQ_HZ;
}

float WindSensor::GetWindDirection()
{
    return directionSensor.GetHeading();
}

AS5600_STATUS_t WindSensor::GetMagnetStatus()
{
    return speedSensor.CheckMagnet();
}
