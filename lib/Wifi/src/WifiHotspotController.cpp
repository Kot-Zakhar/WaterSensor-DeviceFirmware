#include "WifiHotspotController.h"
#include <DNSServer.h>

DNSServer dnsServer;
IPAddress serverIP(192, 168, 100, 1);
IPAddress mask(255,255,255,0);

void initWiFiHotspot() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32 water sensor");

    WiFi.softAPConfig(serverIP, serverIP, mask);

    dnsServer.start(53, "*", serverIP);

    Serial.println(serverIP);
}

IPAddress getHotspotIP() {
    return WiFi.softAPIP();
}

void processWiFiHotspot() {
    dnsServer.processNextRequest();
}