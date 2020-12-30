#ifndef __ORIENTATION_SENSOR_H__
#define __ORIENTATION_SENSOR_H__

#include <Arduino.h>

#include <Adafruit_Sensor_Calibration.h>
#include <Adafruit_AHRS.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>

#include "sensor.h"

namespace FCAero {
    typedef struct {
        float roll, pitch, heading, qw, qx, qy, qz, rollRate; // pitch and heading rate?
    } Orientation;

    class OrientationSensor : public Sensor<Orientation> {
        Adafruit_FXOS8700 fxos;
        Adafruit_FXAS21002C fxas;
        Adafruit_Sensor *accelerometer, *gyroscope, *magnetometer;
        Adafruit_NXPSensorFusion filter;

        #if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)
            Adafruit_Sensor_Calibration_EEPROM cal;
        #else
            Adafruit_Sensor_Calibration_SDFat cal;
        #endif

        uint16_t filterUpdateRate;
        uint32_t timestamp;

    public:
        OrientationSensor(uint16_t filterUpdateRate);
        bool init();
        bool available();
        Orientation read();
        void readInto(Orientation* o);
        void writeReport(Orientation data);
    };
}

#endif
