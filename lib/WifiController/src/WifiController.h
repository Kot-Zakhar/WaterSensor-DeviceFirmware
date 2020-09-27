#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#include <MemoryController.h>
#include <IOController.h>
#include <WiFi.h>


#define CONNECTING_TIMEOUT 5000

void initWiFiController();
void wifiControllerOff();

String getCurrentWiFiSsid();
bool isWiFiConnected();
bool awaitForWiFiConnection(int timeout);
void awaitForWiFiConnection();
void connectToWiFi(const char *ssid, const char *password);
bool connectToAnyWiFiFromMemory();
void disconnectFromWiFi();

void syncTime();
char *getDateTimeStr(char *buffer, size_t length, bool shortFormat = true);
char *getTimeStr(char *buffer, size_t length, bool shortFormat = true);

#endif