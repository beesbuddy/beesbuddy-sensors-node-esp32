#include <sstream>
#include <string>
#include <Arduino.h>
#include <config.h>
#include <Utils.h>
// Modules
#include <Scale.h>
#include <HeatIndex.h>
#include <Communication.h>

// Monitoring settings
#define SerialMon                    Serial
#define MONITOR_BAUD_RATE            115200

// DHT11 settings
#define DHT_DATA_PIN                25
#define DHT_VCC_PIN                 26

// Load cell settings
#define LOADCELL_VCC_PIN            4
#define LOADCELL_DOUT_PIN           19
#define LOADCELL_SCK_PIN            18
#define LOAD_SCALE_FACTOR           22.988
#define LOADCELL_GAIN               128

#ifdef SCALE_MODULE
ScaleModule::Scale scale(
    LOADCELL_DOUT_PIN,
    LOADCELL_SCK_PIN,
    LOADCELL_GAIN,
    LOAD_SCALE_FACTOR,
    DEFAULT_DELAY,
    SerialMon
);
#endif

#ifdef HEAT_INDEX_MODULE
HeatIndexModule::HeatIndex heatIndex(
    DHT_DATA_PIN,
    DHT_VCC_PIN,
    DEFAULT_DELAY,
    SerialMon
);
#endif

#ifdef COMMUNICATION_MODULE
// Initialise static member for upload intervals ???
unsigned long CommunicationModule::Communication::timer = 0;

CommunicationModule::Communication communication(
    SERVICE_UUID,
    APN,
    BAND,
    MQTT_BROKER,
    MQTT_PORT,
    SerialMon
);

std::stringstream stream;
#endif

void setup()
{
    SerialMon.begin(MONITOR_BAUD_RATE);
    randomSeed(analogRead(A0));

#ifdef SCALE_MODULE
    scale.setup();
#else
    SerialMon.println(F("Scale module is disabled."));
#endif

#ifdef HEAT_INDEX_MODULE
    heatIndex.setup();
#else
    SerialMon.println(F("HeatIndex module is disabled."));
#endif

#ifdef COMMUNICATION_MODULE
    communication.setup();
#else
    SerialMon.println(F("Communication module is disabled."));
#endif

}

void loop()
{
    char w[10];
    char t[6];
    char h[6];

#ifdef EMULATE_SENSORS
    sprintf(w, "%8.0f", random(100000, 1000000));
    sprintf(t, "%4.1f", random(10, 100));
    sprintf(h, "%4.1f", random(10, 100));
#endif

#ifdef SCALE_MODULE
    ScaleModule::Data sData = scale.read();
    sprintf(w, "%8.0f", abs(sData.weight));
#endif

#ifdef HEAT_INDEX_MODULE
    HeatIndexModule::Data hData = heatIndex.read();
    sprintf(t, "%4.1f", hData.temperature);
    sprintf(h, "%4.1f", hData.humidity);
#endif

#ifdef COMMUNICATION_MODULE
    communication.write(CLIENT_UUID, MQTT_TOPIC, w, t, h);
    delay(MESSAGE_DELAY);
#endif

    SerialMon.print("received data from sensors: ");
    SerialMon.printf("t: %s, h: %s, w: %s", t, h, w);
    SerialMon.println();
    delay(LOOP_INTERVAL);
}