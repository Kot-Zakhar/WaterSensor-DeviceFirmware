#include "WifiHotspotController.h"
#include <DNSServer.h>

DNSServer dnsServer;

void initWiFiHotspot() {
    WiFi.softAP("ESP32 water sensor");
    dnsServer.start(53, "*", WiFi.softAPIP());

    IPAddress IP = WiFi.softAPIP();
    Serial.println(IP);
}

IPAddress getHotspotIP() {
    return WiFi.softAPIP();
}

void processWiFiHotspot() {
    dnsServer.processNextRequest();
}