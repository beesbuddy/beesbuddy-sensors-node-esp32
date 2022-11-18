#ifndef _HEAT_INDEX_H
#define _HEAT_INDEX_H

#include <Arduino.h>
#include <DHTesp.h>
#include <Core.h>

namespace HeatIndexModule {
    struct Data {
        float temperature;
        float humidity;
        Data()
        {
            temperature = 0.0f;
            humidity = 0.0f;
        }
        explicit Data(const TempAndHumidity &thmd) : temperature(thmd.temperature), humidity(thmd.humidity) {}
    };

    class HeatIndex: Core {
    private:
        DHTesp* dht;
        HardwareSerial *serial;
        uint8_t dataPin;
        uint8_t vccPin;
        long delayInMiliseconds;
        Data d;
    public:
        HeatIndex(uint8_t dataPin, uint8_t vccPin, long delayInMiliseconds, HardwareSerial &serial);
        ~HeatIndex();
        void wakeup();
        void sleep();
        void setup();
        Data read();
    };
}

#endif /* _HEAT_INDEX_H */