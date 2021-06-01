#include <WifiController.h>

const char* failedToObtainTimeMessage = "Failed to otain time.";
const char* failedToObtainTimeShortMessage = "<no time>";

const char* timeFormat = "%T";
const char* timeShortFormat = "%R";
const char* dateTimeFormat = "%a %b %d %T";
const char* dateTimeShortFormat = "%T %D";

const char* ntpServer1 = "by.pool.ntp.org";
const char* ntpServer2 = "europe.pool.ntp.org";
const long  gmtOffset_sec = 10800;
const int   daylightOffset_sec = 0;


void syncTime(){
  // IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Syncing time");
  struct tm timeInfo;
  do {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  } while(!getLocalTime(&timeInfo));
  Serial.println(&timeInfo, "Time synced: %A, %b.%d.%Y %H:%M:%S");
}

char *getDateTimeStr(char *buffer, size_t length, bool shortFormat){
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)){
    strcpy(buffer, shortFormat ? failedToObtainTimeShortMessage : failedToObtainTimeMessage);
  } else {
    strftime(buffer, length,
      shortFormat ? dateTimeShortFormat : dateTimeFormat,
      &timeInfo);
  }
  return buffer;
}

char *getTimeStr(char *buffer, size_t length, bool shortFormat){
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)){
    strcpy(buffer, shortFormat ? failedToObtainTimeShortMessage : failedToObtainTimeMessage);
  } else {
    strftime(buffer, length,
      shortFormat ? timeShortFormat : timeFormat,
      &timeInfo);
  }
  return buffer;
}

void initWiFiController() {
  WiFi.mode(WIFI_STA);
}

void wifiControllerOff(){
  disconnectFromWiFi();
}

void connectToWiFi(const char *ssid, const char *password){
  WiFi.begin(ssid, password);
}

String getCurrentWiFiSsid(){
  return WiFi.SSID();
}

IPAddress getCurrentIP() {
  return WiFi.localIP();
}

void awaitForWiFiConnection(){
  log_v("Awaiting for wifi connection...");
  while (!isWiFiConnected())
  {
    log_v(".");
    delay(200);
  }
}

bool awaitForWiFiConnection(int timeout){
  int time = 0;
  log_v("Awaiting for wifi connection with timeout: %d...", timeout);
  while (!isWiFiConnected() && (time <= timeout))
  {
    log_v(".");
    delay(200);
    time+= 200;
  }
  return isWiFiConnected();
}

bool connectToAnyWiFiFromMemory(){
  char *ssid = (char *)malloc(STRING_LENGTH);
  char *password = (char *)malloc(STRING_LENGTH);
  int amount = getWiFiCredentialsAmountFromMemory();
  bool result = false;
  for (int i = 0; (i < amount) && (!result); i++){
    ssid = getWiFiSsidFromMemory(i, ssid);
    password = getWiFiPasswordFromMemory(i, password);
    log_d("ssid: %s, password: %s", ssid, password);
    connectToWiFi(ssid, password);
    result = awaitForWiFiConnection(5000);
  }
  free(ssid);
  free(password);
  return result;
}

bool isWiFiConnected(){
  return WiFi.status() == WL_CONNECTED;
}

void disconnectFromWiFi(){
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

bool WiFiNotificationIsOn() {
  return getWiFiCredentialsAmountFromMemory() > 0 &&
    getEmailRecipientsAmountFromMemory() > 0;
}

bool WiFiConnectionIsAvailable() {
  log_d("trying to connecto to any wifi");
  initWiFiController();
  bool result = connectToAnyWiFiFromMemory();
  disconnectFromWiFi();
  log_d("%s", result ? "Connection can be established" : "No connection can be established");
  return result;
}


void startServer() {
  WiFiServer httpServer(80);
}