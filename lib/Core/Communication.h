#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <Arduino.h>

#define TINY_GSM_MODEM_SIM7020
#define TINY_GSM_DEBUG               Serial
#define AM7020_BAUD_RATE             115200
#define AM7020_RESET                 5
#define SerialAT                     Serial2
#define MQTT_KEEP_ALIVE              300
#define MQTT_PUBLISH_DELAY           1000

#include <PubSubClient.h>
#include <TinyGsmClient.h>

#include <Core.h>

#ifdef DEBUG_DUMP_AT_COMMAND
#include <StreamDebugger.h>
#endif

namespace CommunicationModule {
struct Data {

};

class Communication: Core {
private:
    TinyGsm *modem;
    PubSubClient *mqttClient;
    TinyGsmClient *tcpClient;
    uint8_t band;
    const char *uuid;
    const char *apn;
    const char *gsmPin;
    const char *brokerName;
    uint16_t brokerPort;
    const char *brokerUsername;
    const char *brokerPassword;
    const char *brokerTopic;
    const char *serviceUUID;
    HardwareSerial *serial;
    void networkConnect();
    void mqttConnect();

public:
    Communication(
        const char *uuid,
        const char *apn,
        uint8_t band,
        const char *brokerName,
        uint16_t brokerPort,
        HardwareSerial &serial
    );
    void mqttCallback(char *topic, byte *payload, u_int64_t len);
    void setup();
    void write(const char *uuid, const char *topic, const char *weight, const char *temp, const char *humidity);
    Data read();
    static unsigned long timer;
    ~Communication();
};
}

#endif /* _COMMUNICATION_H */