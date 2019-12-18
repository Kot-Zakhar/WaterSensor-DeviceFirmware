#pragma once

#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_


#include <MemoryController.h>
#include <IOController.h>
#include <SensorController.h>
#include <ESP32_MailClient.h>


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

bool SendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess = false);

void SyncTime();
char *GetDateTimeStr(char *buffer, size_t length, bool shortFormat = true);
char *GetTimeStr(char *buffer, size_t length, bool shortFormat = true);

#endif