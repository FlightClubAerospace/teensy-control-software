#include "orientation_sensor.h"

namespace FCAero {
    OrientationSensor::OrientationSensor(uint16_t filterUpdateRate) {
        this->filterUpdateRate = filterUpdateRate;
        fxos = Adafruit_FXOS8700(0x8700A, 0x8700B);
        fxas = Adafruit_FXAS21002C(0x0021002C);
    }

    bool OrientationSensor::init() {
        if (!fxos.begin() || !fxas.begin() || !cal.begin() || !cal.loadCalibration()) {
            return false;
        }

        accelerometer = fxos.getAccelerometerSensor();
        gyroscope = &fxas;
        magnetometer = fxos.getMagnetometerSensor();

        filter.begin(filterUpdateRate);

        return true;
    }

    bool OrientationSensor::available() {
        return (millis() - timestamp) >= (1000 / filterUpdateRate);
    }

    void OrientationSensor::readInto(Orientation* o) {
        timestamp = millis();

        sensors_event_t accel, gyro, mag;
        accelerometer->getEvent(&accel);
        gyroscope->getEvent(&gyro);
        magnetometer->getEvent(&mag);

//        cal.calibrate(mag);
//        cal.calibrate(accel);
//        cal.calibrate(gyro);

        float gx = gyro.gyro.x * SENSORS_RADS_TO_DPS,
            gy = gyro.gyro.y * SENSORS_RADS_TO_DPS,
            gz = gyro.gyro.z * SENSORS_RADS_TO_DPS;

        // this is needed, otherwise the return statement fails
        // i don't know either
        Serial.flush();
        filter.update(gx, gy, gz,
                      accel.acceleration.x, accel.acceleration.y, accel.acceleration.z,
                      mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);

//        Orientation o;
        o->heading = filter.getYaw();
        o->pitch = filter.getPitch();
        o->roll = filter.getRoll();
        filter.getQuaternion(&o->qw, &o->qx, &o->qy, &o->qz);
        o->rollRate = gx; // make sure that's the right axis
        Serial.print(gx);
        Serial.print(", ");
        Serial.print(gy);
        Serial.print(", ");
        Serial.println(gz);
//        Serial.println("about to return");
//        return o;
    }

    void OrientationSensor::writeReport(Orientation data) {
        Serial.print("{\"orientation\":{\"heading\":");
        Serial.print(data.heading);
        Serial.print(",\"pitch\":");
        Serial.print(data.pitch);
        Serial.print(",\"roll\":");
        Serial.print(data.roll);
        Serial.print(",\"rollRate\":");
        Serial.print(data.rollRate);
        Serial.println("}}");
    }

    Orientation OrientationSensor::read() {
        Orientation o;
        return o;
    }
}
