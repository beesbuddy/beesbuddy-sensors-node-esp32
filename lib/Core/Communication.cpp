#include <Communication.h>
#include <ArduinoJson.h>

namespace CommunicationModule {
    Communication::Communication(
        const char *uuid,
        const char *apn,
        uint8_t band,
        const char *brokerName,
        uint16_t brokerPort,
        HardwareSerial &serial
    )
    {
        this->uuid = uuid;
        this->apn = apn;
        this->band = band;
        this->brokerName = brokerName;
        this->brokerPort = brokerPort;
        this->serial = &serial;
    };

    void Communication::setup()
    {
    #ifdef DEBUG_DUMP_AT_COMMAND
        StreamDebugger debugger(SerialAT, *serial);
        modem = new TinyGsm(debugger, AM7020_RESET);
    #else
        modem = new TinyGsm(SerialAT, AM7020_RESET);
    #endif
        tcpClient = new TinyGsmClient(*modem);
        mqttClient = new PubSubClient(brokerName, brokerPort, *tcpClient);
        mqttClient->setCallback([this](char *topic, byte * payload, unsigned int len) {
            this->mqttCallback(topic, payload, len);
        });
        mqttClient->setKeepAlive(MQTT_KEEP_ALIVE);
        networkConnect();
    }

    void Communication::mqttCallback(char *topic, byte *payload, u_int64_t len)
    {
        serial->print(F("Message arrived to ["));
        serial->print(topic);
        serial->print(F("]: "));
        serial->write(payload, len);
        serial->println();
    }

    void Communication::write(const char *uuid, const char *topic, const char *weight, const char *temp, const char *humidity)
    {
        serial->println(F("Sending message"));
        
        if (mqttClient && modem) {
            char buffer[1024];

            StaticJsonDocument<1024> doc;
            doc["id"] = uuid;
            doc["w"] = weight;
            doc["t"] = temp;
            doc["h"] = humidity;

            serializeJson(doc, buffer);

            if (!mqttClient->connected()) {
                if (!modem->isNetworkConnected()) {
                    serial->println(F("=== NETWORK NOT connected ==="));
                    networkConnect();
                }

                serial->println(F("===  MQTT NOT connected  ==="));

                mqttConnect();
            }

            // Do not allow to send message to often
            if (millis() >= timer) {
                timer = millis() + MQTT_PUBLISH_DELAY;
                serial->print(F("Publish: "));
                serial->println(buffer);
                mqttClient->publish(topic, buffer);
            }

            mqttClient->loop();
        } else {
            serial->println(F("Communication module is not initialized correctly"));
        }
    }

    void Communication::networkConnect()
    {
        SerialAT.begin(AM7020_BAUD_RATE);

        serial->println(F("Initializing modem..."));

        while (!modem->init() || !modem->nbiotConnect(apn, band)) {};

        serial->println(F(""));
        serial->print(F("Waiting for network"));

        while (!modem->waitForNetwork()) {
            serial->print(F("."));
        }

        serial->println(F(""));
        serial->println(F("Successfully connected to network."));
    }

    void Communication::mqttConnect()
    {
        serial->print(F("Connecting to "));
        serial->print(brokerName);
        serial->println(F("..."));

        String mqttid = ("MQTTID_" + String(uuid));
        
        while (!mqttClient->connect(mqttid.c_str(), "", "")) {
            if (mqttClient->connected()) {
               break; 
            }

            serial->println(F("Failed to connect. Trying again..."));
            delay(3000);
        }

        serial->println(F("Successfuly connected to MQTT."));
    }

    Communication::~Communication()
    {
        delete this->mqttClient;
        delete this->tcpClient;
        delete this->modem;
    }
}