#include "CommandHandlers.h"

#include <MemoryController.h>
#include <NotificationService.h>
#include <SensorsService.h>


int wifiCredsGetAmount() {
    return getWiFiCredentialsAmountFromMemory();
}
struct WiFiCred *wifiCredsGetAll(struct WiFiCred* credsBuffer, int &amount) {
    amount = getWiFiCredentialsAmountFromMemory();
    credsBuffer = (struct WiFiCred*)realloc(credsBuffer, sizeof(struct WiFiCred) * amount);

    for (int i = 0; i < amount; i++) {
        getWiFiSsidFromMemory(i, credsBuffer[i].ssid);
        getWiFiPasswordFromMemory(i, credsBuffer[i].password);
    }

    return credsBuffer;
}
void wifiCredsAdd(struct WiFiCred creds) {
    saveWiFiCredentialsToMemory(creds.ssid, creds.password);
}
void wifiCredsAdd(const char *ssid, const char *password) {
    saveWiFiCredentialsToMemory(ssid, password);
}
void wifiCredsDelete(const char *ssid, const char *password) {
    removeWiFiCredentialsFromMemory(ssid, password);
}
void wifiCredsDelete(int index) {
    removeWiFiCredentialsFromMemory(index);
}
void wifiCredsDelete(const char *ssid) {
    removeWiFiCredentialsFromMemory(ssid);
}
void wifiCredsDeleteAll() {
    removeAllWiFiCredentialsFromMemory();
}


error_t emailServerSettingsGet(email_server_type_t serverType, struct EmailServerSettings &settings) {
    // TODO: check availability
    if (!emailServerSettingsAvailable(serverType))
        return 1;

    getEmailServerSettingsFromMemory(serverType, settings);
    return 0;
}
void emailServerSettingsSet(email_server_type_t serverType, struct EmailServerSettings &settings) {
    saveEmailServerSettingsToMemory(serverType, settings);
}
void emailServerSettingsDelete(email_server_type_t serverType) {
    removeEmailServerSettingsFromMemory(serverType);
}

int emailRecipientsGetAmount() {
    return getEmailRecipientsAmountFromMemory();
}
char *emailRecipientGetByIndex(int index, char *buffer, size_t length) {
    return getEmailRecipientFromMemory(index, buffer, length);
}
void emailRecipientAdd(const char *email) {
    saveEmailRecipientToMemory(email);
}
void emailRecipientDelete(const char *email) {
    removeEmailRecipientFromMemory(email);
}
void emailRecipientDelete(int index) {
    removeEmailRecipientFromMemory(index);
}
void emailRecipientsDeleteAll() {
    removeAllEmailRecipientsFromMemory();
}

int gsmRecipientsGetAmount() {
    return getGsmRecipientsAmountFromMemory();
}
char *gsmRecipientGetByIndex(int index, char *buffer, size_t length) {
    return getGsmRecipientFromMemory(index, buffer, length);
}
void gsmRecipientAdd(const char *phoneNumber) {
    saveGsmRecipientToMemory(phoneNumber);
}
void gsmRecipientDelete(const char *phoneNumber) {
    removeGsmRecipientFromMemory(phoneNumber);
}
void gsmRecipientDelete(int index) {
    removeGsmRecipientFromMemory(index);
}
void gsmRecipientsDeleteAll() {
    removeAllGsmRecipientsFromMemory();
}

char *gsmPinGet(char *buffer, size_t length) {
    getGsmPinFromMemory(buffer, length);
}
void gsmPinSet(const char *pin) {
    saveGsmPinToMemory(pin);
}
void gsmPinDelete() {
    removeGsmPinFromMemory();
}

error_t gprsSettingsGet(struct GprsSettings &settings) {
    if (!gprsSettingsAvailableImMemory)
        return 1;
    getGprsSettingsFromMemory(settings);
    return 0;
}
void gprsSettingsSet(struct GprsSettings &settings) {
    saveGprsSettingsToMemory(settings);
}
void gprsSettingsDelete() {
    removeGprsSettingsFromMemory();
}
bool gprsPermGet() {
    return getGprsPermFromMemory();
}
void gprsPermSet(bool perm) {
    saveGprsPermToMemory(perm);
}

void sendTestSms() {
    notifyAboutEvent(TEST_SMS_NOTIFICATION);
}
void sendTestEmailGprs() {
    notifyAboutEvent(TEST_EMAIL_GPRS_NOTIFICATION);
}
void sendTestEmailWiFi() {
    notifyAboutEvent(TEST_EMAIL_WIFI_NOTIFICATION);
}

bool getWaterSensorBoundaries(int &low, int &high) {
    return getWaterSensorBoundariesFromMemory(low, high);
}
void setWaterSensorBoundaries(int low, int high) {
    saveWaterSensorBoundariesToMemory(low, high);
}
void deleteWaterSensorBoundaries() {
    deleteWaterSensorBoundariesFroMemory();
}
bool getTemperatureBoundaries(float &low, float &high) {
    return getTemperatureBoundariesFromMemory(low, high);
}
void setTemperatureBoundaries(float low, float high) {
    saveTemperatureBoundariesToMemory(low, high);
}
void deleteTemperatureBoundaries() {
    deleteTemperatureBoundariesFroMemory();
}
bool getHumidityBoundaries(float &low, float &high) {
    return getHumidityBoundariesFromMemory(low, high);
}
void setHumidityBoundaries(float low, float high) {
    saveHumidityBoundariesToMemory(low, high);
}
void deleteHumidityBoundaries() {
    deleteHumidityBoundariesFroMemory();
}

void getSensorsValues(SensorsValues &sensors) {
    getCurrentSensorsValues(sensors);
}