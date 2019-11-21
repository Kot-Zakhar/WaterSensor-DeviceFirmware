#pragma once
#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#define CONNECTING_TIMEOUT 5000

#include <ESP32_MailClient.h>

void WiFiControllerInit();
void AwaitForWiFiConnection();
bool IsWiFiConnected();
void ConnectToWiFi(const char *ssid, const char *password);
void DisconnectFromWiFi();
bool SendLetter(const String &recipient, const String &subject, const String &message, bool isHtml);

#endif