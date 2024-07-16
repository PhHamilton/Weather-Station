#ifndef __WINDSENSOR_H__
#define __WINDSENSOR_H__

#include "HMC5883L.h"
#include "AS5600.h"
#include "Arduino.h"

#define ARM_LENGTH 0.1
#define SPEED_MEAS_FREQ_HZ 10

class WindSensor
{
    public:
        WindSensor(uint8_t HMC5883L_Addr, uint8_t AS5600_Addr);
        bool Initialize();
        float GetWindSpeed();
        float GetWindDirection();
        AS5600_STATUS_t GetMagnetStatus();
    private:
        uint16_t buf[SPEED_MEAS_FREQ_HZ];
        HMC5883L_CONFIG_REG_A configA = {
                                      .nAverages      = HMC5883L_AVG_SAMPLES_t::AVG_8,
                                      //.outputDataRate = HMC5883L_ODR_t::ODR_15,
                                      .outputDataRate = 4,
                                      .mode           = HMC5883L_NORMAL
                                    };

        HMC5883L_CONFIG_REG_B configB = {.gain = HMC5883L_GAIN_t::GAIN_1_3};

        HMC5883L_CONFIG_t config = {
                                      .confA = configA,
                                      .confB = configB
                                   };

        HMC5883L_AXES_t axes;
        HMC5883L directionSensor;
        AS5600 speedSensor;
        uint64_t previousMillis = 0;
        uint64_t currentMillis;
        int16_t prevSpeed = 0;
};
#endif //__WINDSENSOR_H__
