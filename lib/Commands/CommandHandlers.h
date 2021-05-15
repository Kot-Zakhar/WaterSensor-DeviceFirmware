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
bool getTemperatureBoundaries(float &low, float &high);
void setTemperatureBoundaries(float low, float high);
void deleteTemperatureBoundaries();
bool getHumidityBoundaries(float &low, float &high);
void setHumidityBoundaries(float low, float high);
void deleteHumidityBoundaries();

void getSensorsValues(SensorsValues &sensors);

// void gsmPinSetCommandHandler(int payload);
// void gsmUseNetworkPermissionGetCommandHandler(int payload);
// void gsmUseNetworkPermissionSetCommandHandler(int payload);
// void gsmRecipientsGetCommandHandler(int payload);
// void gsmRecipientsAddCommandHandler(int payload);
// void gsmRecipientsDeleteSingleCommandHandler(int payload);
// void gsmRecipientsDeleteAllCommandHandler(int payload);

#endif