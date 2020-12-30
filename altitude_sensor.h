#ifndef __ALTITUDE_SENSOR_H__
#define __ALTITUDE_SENSOR_H__

#include <Arduino.h>

#include <Adafruit_DPS310.h>

#include "sensor.h"

namespace FCAero {
    class AltitudeSensor : public Sensor<float> {
        // sea level pressure in hPa
        constexpr static float P0 = 1013.25;
        Adafruit_DPS310 dps;
        dps310_rate_t sampleRate;
        dps310_oversample_t oversampling;

    public:
        AltitudeSensor(dps310_rate_t sampleRate, dps310_oversample_t oversampling);
        bool init();
        bool available();
        float read();
        void writeReport(float data);
    };
}

#endif
