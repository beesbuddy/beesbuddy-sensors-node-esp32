#include <sstream>
#include <string>
#include <Arduino.h>
#include <config.h>
#include <Utils.h>
// Modules
#include <Scale.h>
#include <HeatIndex.h>
#include <Communication.h>

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

#ifdef SCALE_MODULE
    ScaleModule::Data sData = scale.read();
    sprintf(w, "%8.0f", abs(sData.weight));
#endif

#ifdef HEAT_INDEX_MODULE
    HeatIndexModule::Data hData = heatIndex.read();
    sprintf(t, "%4.1f", hData.temperature);
    sprintf(h, "%4.1f", hData.humidity);
#endif

#ifdef EMULATE_SENSORS
    sprintf(h, "%8.0f", random(1000000));
    sprintf(t, "%4.1f", random(100)); 
    sprintf(h, "%4.1f", random(100)); 
#endif

#ifdef COMMUNICATION_MODULE
    communication.write(SERVICE_UUID, MQTT_TOPIC, w, t, h);
    delay(MESSAGE_DELAY);
#endif

    delay(LOOP_INTERVAL);
}