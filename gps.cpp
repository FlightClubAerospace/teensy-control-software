#include <math.h>

#include "gps.h"

namespace FCAero {
    GPS::GPS(HardwareSerial *ser) {
        ser->begin(9600);
        this->gps = new Adafruit_GPS(ser);
    }

    bool GPS::init() {
        gps->begin(9600);
        gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
        gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
        return true; // should actually ask for firmware version (PMTK_Q_RELEASE) and verify response
    }

    bool GPS::available() {
        // we should also flush GPS here; read a char at a time until nothing is available
        while (gps->available()) {
            gps->read();
        }
        return gps->newNMEAreceived() && gps->parse(gps->lastNMEA());
    }

    GPSPosition GPS::read() {
        GPSPosition pos;

        // if GPS.latitude = 12220.3, actual latitude is 122° 20.3'
        pos.latDeg = gps->latitude / 100;
        pos.longDeg = gps->longitude / 100;
        pos.latMin = fabsf(fmodf(gps->latitude, 100.0f));
        pos.longMin = fabsf(fmodf(gps->longitude, 100.0f));
        pos.satellites = gps->satellites;

        return pos;
    }

    void GPS::writeReport(GPSPosition data) {
        Serial.print("{\"position\":{\"latDeg\":");
        Serial.print(data.latDeg);
        Serial.print(",\"latMin\":");
        Serial.print(data.latMin);
        Serial.print(",\"longDeg\":");
        Serial.print(data.longDeg);
        Serial.print(",\"longMin\":");
        Serial.print(data.longMin);
        Serial.print(",\"satellites\":");
        Serial.print(data.satellites);
        Serial.println("}}");
    }
}
