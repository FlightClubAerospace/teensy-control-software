#include <Arduino.h>

#include "orientation_sensor.h"
#include "altitude_sensor.h"
#include "gps.h"
#include "airspeed_sensor.h"

using namespace FCAero;

AltitudeSensor altitude(DPS310_16HZ, DPS310_128SAMPLES);
OrientationSensor orientation(5);
GPS gps(&Serial1);
AirspeedSensor airspeed;

void setup() {
    Serial.begin(115200);
    while (!Serial) yield();

    if (!altitude.init()) {
        Serial.println("Failed to initialize altitude");
        while (1) delay(500);
    }

    if (!orientation.init()) {
        Serial.println("Failed to initialize orientation");
        while (1) delay(500);
    }

    if (!gps.init()) {
        Serial.println("Failed to initialize GPS");
        while (1) delay(500);
    }

    if (!airspeed.init()) {
        Serial.println("Failed to initialize airspeed");
        while (1) delay(500);
    }

    Serial.println("Sensors OK and GPS OK!!!");

    // run I2C at 400KHz
    Wire.setClock(400000);
}

void loop() {
    Orientation o;

    if (altitude.available()) {
        float alt = altitude.read();
        altitude.writeReport(alt);
    }

    if (orientation.available()) {
        orientation.readInto(&o);
        orientation.writeReport(o);
    }

    if (gps.available()) {
        gps.available();
        GPSPosition p = gps.read();
        gps.writeReport(p);
    }

    if (airspeed.available()) {
        float speed = airspeed.read();
        airspeed.writeReport(speed);
    }

    delay(1000);
}
