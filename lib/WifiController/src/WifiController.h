#pragma once
#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#define CONNECTING_TIMEOUT 5000

#include <ESP32_MailClient.h>
#include <MemoryController.h>


void InitWiFiController();
void WiFiControllerOff();

String GetCurrentWiFiSsid();
bool IsWiFiConnected();
bool AwaitForWiFiConnection(int timeout);
void AwaitForWiFiConnection();
void ConnectToWiFi(const char *ssid, const char *password);
bool ConnectToAnyWiFiFromMemory();
void DisconnectFromWiFi();
bool SendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess = false);

#endif