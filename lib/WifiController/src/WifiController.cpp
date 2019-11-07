#include <WifiController.h>
#include <WiFi.h>

const char* host = "api.noopschallenge.com";

void ConnectToWiFi(const char *ssid, const char *password){
    WiFi.begin(ssid, password);
    // delay(500);
}

bool IsWiFiConnected(){
    return WiFi.status() == WL_CONNECTED;
}