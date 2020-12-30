#include <math.h>

#include "altitude_sensor.h"

namespace FCAero {
    AltitudeSensor::AltitudeSensor(dps310_rate_t sampleRate, dps310_oversample_t oversampling) {
        this->sampleRate = sampleRate;
        this->oversampling = oversampling;
    }

    bool AltitudeSensor::init() {
        if (!dps.begin_I2C()) {
            return false;
        }

        dps.configurePressure(sampleRate, oversampling);
        dps.configureTemperature(sampleRate, oversampling);

        return true;
    }

    bool AltitudeSensor::available() {
        return dps.temperatureAvailable() && dps.pressureAvailable();
    }

    float AltitudeSensor::read() {
        sensors_event_t temp_event, pressure_event;
        dps.getEvents(&temp_event, &pressure_event);
        float temp = temp_event.temperature, pressure = pressure_event.pressure;

        // https://physics.stackexchange.com/questions/333475/how-to-calculate-altitude-from-current-temperature-and-pressure
        float alt = (powf(P0 / pressure, 0.190222f) - 1) * (temp + 273.15f) * 153.846f;
        return alt;
    }

    void AltitudeSensor::writeReport(float data) {
        Serial.print("{\"altitude\":");
        Serial.print(data);
        Serial.println('}');
    }
}
