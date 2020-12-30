#ifndef __GPS_H__
#define __GPS_H__

#include <Arduino.h>

#include <Adafruit_GPS.h>

#include "sensor.h"

namespace FCAero {
    typedef struct {
        int16_t latDeg, longDeg;
        float latMin, longMin;
        uint8_t satellites;
    } GPSPosition;

    class GPS : public Sensor<GPSPosition> {
        Adafruit_GPS *gps;

    public:
        GPS(HardwareSerial *ser);
        bool init();
        bool available();
        GPSPosition read();
        void writeReport(GPSPosition data);
    };
}

#endif
