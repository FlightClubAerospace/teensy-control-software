#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <Arduino.h>

namespace FCAero {
    template<typename T>
    class Sensor {
    public:
        virtual bool init();
        virtual bool available();
        virtual T read();
        virtual void writeReport(T data);
    };
}

#endif
