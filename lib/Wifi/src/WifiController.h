#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#include <MemoryController.h>
#include <IOController.h>
#include <WiFi.h>


#define CONNECTING_TIMEOUT 5000

void wifiControllerOff();

String getCurrentWiFiSsid();
IPAddress getCurrentIP();
bool isWiFiConnected();
void initWiFiController();
bool awaitForWiFiConnection(int timeout);
void awaitForWiFiConnection();
void connectToWiFi(const char *ssid, const char *password);
bool connectToAnyWiFiFromMemory();
void disconnectFromWiFi();

bool WiFiNotificationIsOn();
bool WiFiConnectionIsAvailable();

void syncTime();
char *getDateTimeStr(char *buffer, size_t length, bool shortFormat = true);
char *getTimeStr(char *buffer, size_t length, bool shortFormat = true);

#endif