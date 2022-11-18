#include <Scale.h>
#include <Arduino.h>

namespace ScaleModule {
    Scale::Scale(uint8_t dout, uint8_t pdSck, uint8_t gain, float scaleFactor, long delayInMiliseconds, HardwareSerial &serial)
    {
        this->dout = dout;
        this->pdSck = pdSck;
        this->gain = gain;
        this->scaleFactor = scaleFactor;
        this->delayInMiliseconds = delayInMiliseconds;
        this->serial = &serial;
    }

    void Scale::setup()
    {
        scale = new HX711();
        
        scale->begin(dout, pdSck, gain);

        if (scaleFactor == 0.0f) {
            doCalibration();
        }

        scale->set_scale(scaleFactor);
        scale->tare();
        serial->println(F("Scale module initiated"));
    }

    void Scale::setScaleFactor(const float scaleFactor)
    {
        this->scaleFactor = scaleFactor;
    }

    void Scale::doCalibration()
    {
        scale->set_scale();
        scale->tare();
        // TODO: Implement full calibration logic
    }

    Data Scale::read()
    {
        if (scale->is_ready()) {
            d.weight = scale->get_units(5);
        } else {
            serial->println(F("HX711 not ready yet!!!"));
        }

        return d;
    }

    Scale::~Scale()
    {
        scale->set_scale();
        scale->tare();
        delete scale;
    }
}
