#include "WifiHotspotController.h"
#include <WiFi.h>
#include <DNSServer.h>

DNSServer dnsServer;

void initWiFiHotspot() {
    WiFi.softAP("ESP32 water sensor");
    dnsServer.start(53, "*", WiFi.softAPIP());

    IPAddress IP = WiFi.softAPIP();
    Serial.println(IP);
}

void processWiFiHotspot() {
    dnsServer.processNextRequest();
}