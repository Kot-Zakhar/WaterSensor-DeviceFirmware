#include <WifiController.h>

SMTPData smtpData;

//const char* host = "api.noopschallenge.com";

void InitWiFiController(){
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
  log_v("Email:\n ---\n Smtp server: %s\n Smtp port: %s\n Smtp login: %s\n Smtp sender name: %s\n ---\n Recipient: %s\n Subject: %s\n Message:\n %s\n ---\n",
    server,
    port,
    login,
    sender,
    recipient,
    subject,
    message
  );

  // AwaitForWiFiConnection();

  smtpData.setDebug(true);

  smtpData.setLogin(server, String(port).toInt(), login, password);
  smtpData.setSender(sender, login);
  smtpData.setPriority("High");
  smtpData.setSubject(subject);
  smtpData.setMessage(message, isHtml);
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