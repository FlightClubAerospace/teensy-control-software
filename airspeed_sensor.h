#ifndef __AIRSPEED_SENSOR_H__
#define __AIRSPEED_SENSOR_H__

#include <Arduino.h>

#include "sensor.h"

namespace FCAero {
    class AirspeedSensor : public Sensor<float> {
        constexpr static float P_min = -1.0f;
        constexpr static float P_max = 1.0f;
        constexpr static float PSI_to_Pa = 6894.757f;

    public:
        bool init();
        bool available();
        float read();
        void writeReport(float data);
    };
}

#endif
