#pragma once
#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#define CONNECTING_TIMEOUT 5000

#include <WiFi.h>
#include <ESP32_MailClient.h>
#include <Arduino.h>

bool IsWiFiConnected();
void ConnectToWiFi(const char *ssid, const char *password);
void DisconnectFromWiFi();
bool SendLetter(String recipient, String subject, String message, bool isHtml);

#endif