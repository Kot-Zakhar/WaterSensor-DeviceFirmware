#include <WifiController.h>

SMTPData smtpData;
IMAPData imapData;

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

const String SearchCriteria = "UID SEARCH SUBJECT \"esp32\" TEXT \"check\" UNSEEN";

//const char* host = "api.noopschallenge.com";

void InitWiFiController(){
}

void SyncTime(){
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Syncing time");
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
  if (!EmailValuesAvailable()){
    log_v("Not all email settings are set!");
    return false;
  }
  if (!IsWiFiConnected()){
    log_v("Wifi is not connected.");
    return false;
  }
  char *server = GetEmailValue(EMAIL_SMTP_SERVER, (char *)malloc(STRING_LENGTH));
  char *port = GetEmailValue(EMAIL_SMTP_PORT, (char *)malloc(STRING_LENGTH));
  char *login = GetEmailValue(EMAIL_LOGIN, (char *)malloc(STRING_LENGTH));
  char *password = GetEmailValue(EMAIL_PASS, (char *)malloc(STRING_LENGTH));
  char *sender = GetEmailValue(EMAIL_SMTP_SENDER, (char *)malloc(STRING_LENGTH));
  char *recipient = GetEmailValue(EMAIL_SMTP_RECIPIENT, (char *)malloc(STRING_LENGTH));
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

bool CheckForIncomingLetter(){
  log_i("Checking incomming letter");
  if (!EmailValuesAvailable()){
    log_v("Not all email settings are set!");
    return false;
  }
  bool result = false;

  char *server = GetEmailValue(EMAIL_IMAP_SERVER, (char *)malloc(STRING_LENGTH));
  char *port = GetEmailValue(EMAIL_IMAP_PORT, (char *)malloc(STRING_LENGTH));
  char *login = GetEmailValue(EMAIL_LOGIN, (char *)malloc(STRING_LENGTH));
  char *password = GetEmailValue(EMAIL_PASS, (char *)malloc(STRING_LENGTH));

  imapData.setLogin(server, String(port).toInt(), login, password);
  imapData.setSearchCriteria(SearchCriteria);
  imapData.setRecentSort(true);
  imapData.setSearchLimit(1);

  if (MailClient.readMail(imapData) && imapData.availableMessages())
  {
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Got check mail.");
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Replying...");
    
    MailClient.setFlag(imapData, imapData.getUID(0).toInt(), "\\Seen");

    String message = String("Got the message. I'm working fine :)\n") +
      "Sensor value is: " + GetSensorValue();
    String subject = "Reply to \"" +
      imapData.getSubject(0) + "\"";

    result = SendLetter(subject.c_str(), message.c_str(), false);
    if (result){
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Replied successfully.");
    } else {
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Could not reply.");
    }
      
  } else {
    log_i("No check letters in mail detected.");
  }

  free(password);
  free(login);
  free(port);
  free(server);

  return result;
}