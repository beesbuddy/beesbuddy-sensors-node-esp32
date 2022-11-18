#ifndef _CONFIG_H
#define _CONFIG_H

#define SCALE_MODULE
// #define BLE_MODULE
#define COMMUNICATION_MODULE
#define HEAT_INDEX_MODULE

#define APP_NAME                            "QueenBee-61f93acc"
#define SERVICE_UUID                        "2f06c96c-4e48-11ed-bdc3-0242ac120002"
#define CLIENT_UUID                         "61f93acc-53bf-11ed-bdc3-0242ac120002"
#define TEMP_CHARACTERISTIC_UUID            "c392aef2-4e48-11ed-bdc3-0242ac120002"
#define HUMIDITY_CHARACTERISTIC_UUID        "e457b18c-4e48-11ed-bdc3-0242ac120002"
#define WEIGHT_CHARACTERISTIC_UUID          "eafc6b18-4e48-11ed-bdc3-0242ac120002"

#define UPLOAD_INTERVAL              5000
#define LOOP_INTERVAL                5000
#define DEFAULT_DELAY                1000
#define MESSAGE_DELAY                2000

#define SerialMon                    Serial
#define MONITOR_BAUD_RATE            115200

// Network settings
#define GSM_PIN                     "0000"
#define APN                         "omnitel"
#define BAND                        20

// MQTT settings
#define MQTT_BROKER                 "test.mosquitto.org"
#define MQTT_PORT                   1883
#define MQTT_USERNAME               ""
#define MQTT_PASSWORD               ""
#define MQTT_TOPIC                  "beesbuddy/" SERVICE_UUID "/metrics"
#define MQTT_KEEP_ALIVE             300

// Load cell settings
#define DHT_DATA_PIN                25
#define DHT_VCC_PIN                 26

#define LOADCELL_VCC_PIN            4
#define LOADCELL_DOUT_PIN           19
#define LOADCELL_SCK_PIN            18
#define LOAD_SCALE_FACTOR           22.988
#define LOADCELL_GAIN               128

#endif /* _CONFIG_H */