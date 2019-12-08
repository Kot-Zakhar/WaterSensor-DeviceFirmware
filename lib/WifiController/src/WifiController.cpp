#include <WifiController.h>

SMTPData smtpData;

const char* failedToObtainTimeMessage = "Failed to otain time.";
const char* failedToObtainTimeShortMessage = "<no time>";

const char* timeFormat = "%T";
const char* timeShortFormat = "%R";
const char* dateTimeFormat = "%a %b %d %T";
const char* dateTimeShortFormat = "%T %D";

const char* htmlDateTimeLeftSide = "<br><p>";
const char* htmlDateTimeRightSize = "</p><br>";

const char* ntpServer1 = "by.pool.ntp.org";
const char* ntpServer2 = "europe.pool.ntp.org";
const long  gmtOffset_sec = 10800;
const int   daylightOffset_sec = 0;

//const char* host = "api.noopschallenge.com";

void InitWiFiController(){
}

void SyncTime(){
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

bool SendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess){
  log_v("Sending letter");
  if (!SmtpValuesAvailable()){
    log_v("Not all smtp settings are set!");
    return false;
  }
  char *server = GetSmtpValue(SMTP_SERVER, (char *)malloc(STRING_LENGTH));
  char *port = GetSmtpValue(SMTP_PORT, (char *)malloc(STRING_LENGTH));
  char *login = GetSmtpValue(SMTP_LOGIN, (char *)malloc(STRING_LENGTH));
  char *password = GetSmtpValue(SMTP_PASS, (char *)malloc(STRING_LENGTH));
  char *sender = GetSmtpValue(SMTP_SENDER, (char *)malloc(STRING_LENGTH));
  char *recipient = GetSmtpValue(SMTP_RECIPIENT, (char *)malloc(STRING_LENGTH));
  char *dateTime = GetDateTimeStr((char *)malloc(STRING_LENGTH), STRING_LENGTH, false);
  
  String timeStampedMessage = 
    String(message) +
    (isHtml ? htmlDateTimeLeftSide : "\n") + 
    dateTime +
    (isHtml ? htmlDateTimeRightSize: "\n");

  log_v("Email:\n ---\n Smtp server: %s\n Smtp port: %s\n Smtp login: %s\n Smtp sender name: %s\n ---\n Recipient: %s\n Subject: %s\n Message:\n %s\n ---\n",
    server,
    port,
    login,
    sender,
    recipient,
    subject,
    timeStampedMessage.c_str()
  );

  smtpData.setDebug(true);

  smtpData.setLogin(server, String(port).toInt(), login, password);
  smtpData.setSender(sender, login);
  smtpData.setPriority("High");
  smtpData.setSubject(subject);
  smtpData.setMessage(timeStampedMessage, isHtml);
  smtpData.addRecipient(recipient);

  bool result = false;
  do {
    log_v("Trying to send email..." );
    result = MailClient.sendMail(smtpData);
    if (result)
      log_v("Successfully sent.");
    else
      log_v("Not sent.");
  } while(!result && retryUntilSuccess);

  smtpData.empty();

  free(server);
  free(port);
  free(login);
  free(password);
  free(sender);
  free(recipient);

  return result;
}