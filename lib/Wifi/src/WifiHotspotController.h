#ifndef _WIFI_HOTSPOT_CONTROLLER_H_
#define _WIFI_HOTSPOT_CONTROLLER_H_
#include <WiFi.h>

void initWiFiHotspot();
void processWiFiHotspot();
IPAddress getHotspotIP();

#endif