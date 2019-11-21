#include <WifiController.h>

SMTPData smtpData;

//const char* host = "api.noopschallenge.com";

void WiFiControllerInit(){
}

void ConnectToWiFi(const char *ssid, const char *password){
    WiFi.begin(ssid, password);
    // delay(500);
}

void AwaitForWiFiConnection(){
  Serial.println("Awaiting for wifi connection...");
  while (!IsWiFiConnected())
  {
    Serial.print(".");
    delay(200);
  }
}

bool IsWiFiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

void DisconnectFromWiFi(){
    WiFi.disconnect();
}

//Callback function to get the Email sending status
void sendCallback(SendStatus msg)
{
  //Print the current status
  Serial.println(msg.info());

  //Do something when complete
  if (msg.success())
  {
    Serial.println("----------------");
  }
}

bool SendLetter(const String &recipient, const String &subject, const String &message, bool isHtml){
    Serial.println("recipient: " + recipient + "\nsubject: " + subject + "\nmessage: " + message);

    AwaitForWiFiConnection();

    // smtpData.setDebug(true);

    smtpData.setLogin(PERSONAL_SMTP_SERVER, PERSONAL_SMTP_PORT, PERSONAL_SMTP_LOGIN, PERSONAL_SMTP_PASSWORD);
    smtpData.setSender("ESP32", "kot.zakhar@gmail.com");
    smtpData.setPriority("High");
    // smtpData.setSubject(subject.c_str());
    // smtpData.setMessage(message.c_str(), isHtml);
    // smtpData.addRecipient(recipient.c_str());

    //Set the subject
    smtpData.setSubject(subject);

    //Set the message - normal text or html format
    smtpData.setMessage(message, isHtml);

    //Add recipients, can add more than one recipient
    smtpData.addRecipient(recipient);

    smtpData.addCustomMessageHeader("Date: Sat, 10 Aug 2019 21:39:56 -0700 (PDT)");
    
    smtpData.setSendCallback(sendCallback);

    bool result = MailClient.sendMail(smtpData);
    if (!result){
        Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    }
    smtpData.empty();
    return result;
}