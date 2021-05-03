#include "CommandHandlers.h"

#include <MemoryController.h>

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