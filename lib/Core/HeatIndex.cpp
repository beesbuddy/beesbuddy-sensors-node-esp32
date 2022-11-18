#include <HeatIndex.h>

namespace HeatIndexModule {
    HeatIndex::HeatIndex(uint8_t dataPin, uint8_t vccPin, long delayInMiliseconds, HardwareSerial &serial)
    {
        this->dataPin = dataPin;
        this->vccPin = vccPin;
        this->delayInMiliseconds = delayInMiliseconds;
        this->serial = &serial;
    }

    void HeatIndex::setup()
    {
        pinMode(vccPin, OUTPUT);
        dht = new DHTesp();
        dht->setup(dataPin, DHTesp::DHT11);
        serial->println(F("HeatIndex module initiated"));
    }

    void HeatIndex::wakeup()
    {
        digitalWrite(vccPin, HIGH);
        delay(delayInMiliseconds);
    }

    void HeatIndex::sleep()
    {
        digitalWrite(vccPin, LOW);
    }

    Data HeatIndex::read()
    {
        serial->println("Reading metrics");
        wakeup();
        TempAndHumidity values = dht->getTempAndHumidity();
        d.humidity = values.humidity;
        d.temperature = values.temperature;
        sleep();

        return d;
    }

    HeatIndex::~HeatIndex()
    {
        sleep();
        delete dht;
    }
}
