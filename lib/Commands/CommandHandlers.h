#ifndef _COMMAND_HANDLERS_H_
#define _COMMAND_HANDLERS_H_

#include <custom_defaults.h>
#include <custom_types.h>
#include <errno.h>

struct WiFiCred *wifiCredsGetAll(struct WiFiCred *credsBuffer, int &amount);
int wifiCredsGetAmount();
void wifiCredsAdd(struct WiFiCred creds);
void wifiCredsAdd(const char *ssid, const char *password);
void wifiCredsDelete(const char *ssid, const char *password);
void wifiCredsDelete(const char *ssid);
void wifiCredsDelete(int index);
void wifiCredsDeleteAll();

// char *pingCommandHandler(char *responseBuffer, size_t len);

error_t emailServerSettingsGet(email_server_type_t serverType, struct EmailServerSettings &settings);
void emailServerSettingsSet(email_server_type_t serverType, struct EmailServerSettings &settings);
void emailServerSettingsDelete(email_server_type_t serverType);

int emailRecipientsGetAmount();
char *emailRecipientGetByIndex(int index, char *buffer, size_t length);
void emailRecipientAdd(const char *email);
void emailRecipientDelete(const char *email);
void emailRecipientDelete(int index);
void emailRecipientsDeleteAll();

char *gsmPinGet(char *buffer, size_t length);
void gsmPinSet(const char *pin);
void gsmPinDelete();

int gsmRecipientsGetAmount();
char *gsmRecipientGetByIndex(int index, char *buffer, size_t length);
void gsmRecipientAdd(const char *phoneNumber);
void gsmRecipientDelete(const char *phoneNumber);
void gsmRecipientDelete(int index);
void gsmRecipientsDeleteAll();

error_t gprsSettingsGet(struct GprsSettings &settings);
void gprsSettingsSet(struct GprsSettings &settings);
void gprsSettingsDelete();
bool gprsPermGet();
void gprsPermSet(bool perm);

void sendTestSms();
void sendTestEmailGprs();
void sendTestEmailWiFi();

bool getWaterSensorBoundaries(int &low, int &high);
void setWaterSensorBoundaries(int low, int high);
void deleteWaterSensorBoundaries();
bool getTemperatureBoundaries(int &low, int &high);
void setTemperatureBoundaries(int low, int high);
void deleteTemperatureBoundaries();
bool getHumidityBoundaries(int &low, int &high);
void setHumidityBoundaries(int low, int high);
void deleteHumidityBoundaries();

void getSensorsValues(SensorsValues &sensors);

void switchStateToNext();
void showNextScreenPage();

#endif