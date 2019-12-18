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

void InitWiFiController(){
}

void SyncTime(){
  // IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Syncing time");
  struct tm timeInfo;
  do {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  } while(!getLocalTime(&timeInfo));
  Serial.println(&timeInfo, "Time synced: %A, %b.%d.%Y %H:%M:%S");
}

char *GetDateTimeStr(char *buffer, size_t length, bool shortFormat){
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

char *GetTimeStr(char *buffer, size_t length, bool shortFormat){
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

void WiFiControllerOff(){
  DisconnectFromWiFi();
}

void ConnectToWiFi(const char *ssid, const char *password){
  WiFi.begin(ssid, password);
}

String GetCurrentWiFiSsid(){
  return WiFi.SSID();
}

void AwaitForWiFiConnection(){
  log_v("Awaiting for wifi connection...");
  while (!IsWiFiConnected())
  {
    log_v(".");
    delay(200);
  }
}

bool AwaitForWiFiConnection(int timeout){
  int time = 0;
  log_v("Awaiting for wifi connection...");
  while (!IsWiFiConnected() && (time <= timeout))
  {
    log_v(".");
    delay(200);
    time+= 200;
  }
  return IsWiFiConnected();
}

bool ConnectToAnyWiFiFromMemory(){
  char *ssid = (char *)malloc(STRING_LENGTH);
  char *password = (char *)malloc(STRING_LENGTH);
  int amount = GetWiFiCredentialsAmountFromMemory();
  bool result = false;
  for (int i = 0; (i < amount) && (!result); i++){
    ConnectToWiFi(GetWiFiSsidFromMemory(i, ssid), GetWiFiPasswordFromMemory(i, password));
    result = AwaitForWiFiConnection(2000);
  }
  free(ssid);
  free(password);
  return result;
}

bool IsWiFiConnected(){
  return WiFi.status() == WL_CONNECTED;
}

void DisconnectFromWiFi(){
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}
