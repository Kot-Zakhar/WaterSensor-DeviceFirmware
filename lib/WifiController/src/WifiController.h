#pragma once
#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#define CONNECTING_TIMEOUT 1000

bool IsWiFiConnected();
void ConnectToWiFi(const char *ssid, const char *password);

#endif