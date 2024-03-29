#pragma once

#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#include <MemoryController.h>
#include <IOController.h>
#include <WiFi.h>


#define CONNECTING_TIMEOUT 5000

void InitWiFiController();
void WiFiControllerOff();

String GetCurrentWiFiSsid();
bool IsWiFiConnected();
bool AwaitForWiFiConnection(int timeout);
void AwaitForWiFiConnection();
void ConnectToWiFi(const char *ssid, const char *password);
bool ConnectToAnyWiFiFromMemory();
void DisconnectFromWiFi();

void SyncTime();
char *GetDateTimeStr(char *buffer, size_t length, bool shortFormat = true);
char *GetTimeStr(char *buffer, size_t length, bool shortFormat = true);

#endif