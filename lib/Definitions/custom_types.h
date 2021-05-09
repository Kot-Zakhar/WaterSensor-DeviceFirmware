#ifndef _TYPES_H_
#define _TYPES_H_

#include "custom_defaults.h"

struct WiFiCred{
    char ssid[STRING_LENGTH];
    char password[STRING_LENGTH];
};

struct EmailServerSettings {
    char server[STRING_LENGTH];
    char port[STRING_LENGTH];
    char login[STRING_LENGTH];
    char password[STRING_LENGTH];
    bool ssl;
};

struct GprsSettings {
    char apn[STRING_LENGTH];
    char user[STRING_LENGTH];
    char password[STRING_LENGTH];
};



enum device_state_t {
    DEVICE_STATE_WORK = 1,
    
    DEVICE_STATE_CONFIG = 2,

    DEVICE_STATE_CONFIG_BLUETOOTH = 4 ^ DEVICE_STATE_CONFIG,
    DEVICE_STATE_CONFIG_WIFI_HOTSPOT = 8 ^ DEVICE_STATE_CONFIG
};

enum email_server_type_t {
    SMTP_EMAIL_SERVER_TYPE,
    IMAP_EMAIL_SERVER_TYPE
};

enum email_server_settings_t {
    EMAIL_SERVER_ADDRESS,
    EMAIL_SERVER_PORT,
    EMAIL_SERVER_LOGIN,
    EMAIL_SERVER_PASSWORD,
    EMAIL_SERVER_SSL
};

enum gprs_settings_t {
    GPRS_APN,
    GPRS_USER,
    GPRS_PASSWORD
};

enum sensor_type_t {
    WATER_SENSOR_TYPE = 2,
    TEMPERATURE_SENSOR_TYPE = 4,
    HUMIDITY_SENSOR_TYPE = 8
};

struct SensorsValues {
    int water;
    float temperature;
    float humidity;
};

#endif