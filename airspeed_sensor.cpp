#include <Wire.h>

#include "airspeed_sensor.h"

namespace FCAero {
    bool AirspeedSensor::init() {
        // should check if I2C device is present
        return true;
    }

    bool AirspeedSensor::available() {
        return true;
    }

    float AirspeedSensor::read() {
        Wire.requestFrom(0x28, 4);
        uint8_t val[4] = {Wire.read(), Wire.read(), Wire.read(), Wire.read()};

        int16_t dp_raw = 0;
        dp_raw = (val[0] << 8) + val[1];
        dp_raw = 0x3FFF & dp_raw;

        float diff_press_PSI = -((dp_raw - 0.1f * 16383) * (P_max - P_min) / (0.8f * 16383) + P_min);
        float diff_press_pa_raw = diff_press_PSI * PSI_to_Pa;

        float vel = sqrt(2.0f * diff_press_pa_raw / 1.1839);
        return vel;
    }

    void AirspeedSensor::writeReport(float data) {
        Serial.print("{\"airspeed\":");
        Serial.print(data);
        Serial.println('}');
    }
}
