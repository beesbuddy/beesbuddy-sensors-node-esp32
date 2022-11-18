#ifndef _SCALE_H
#define _SCALE_H

#include <HX711.h>
#include <Arduino.h>

#include <Core.h>

namespace ScaleModule {
    struct Data {
        float weight;
        Data()
        {
            weight = 0.0f;
        }
    };

    class Scale: Core {
    private:
        HX711 *scale;
        HardwareSerial *serial;
        Data d;
        uint8_t dout;
        uint8_t pdSck;
        uint8_t gain;
        float scaleFactor;
        long delayInMiliseconds;
    public:
        Scale(uint8_t dout, uint8_t pdSck, uint8_t gain, float scaleFactor, long delayInMiliseconds, HardwareSerial &serial);
        ~Scale();
        void setup();
        void doCalibration();
        void setScaleFactor(const float scaleFactor);
        Data read();
    };
}

#endif /* _SCALE_H */