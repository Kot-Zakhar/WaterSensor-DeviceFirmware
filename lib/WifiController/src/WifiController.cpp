#include <WifiController.h>
// #include <esp_wifi.h>


//const char* host = "api.noopschallenge.com";

ESP32_MailClient client;

void WiFiControllerInit(){
    client = ESP32_MailClient();
}

void ConnectToWiFi(const char *ssid, const char *password){
    WiFi.begin(ssid, password);
    // delay(500);
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

bool SendLetter(String recipient, String subject, String message, bool isHtml){
    Serial.println("recipient: " + recipient + "\nsubject: " + subject + "\nmessage: " + message);
    SMTPData smtpData;

    smtpData.setDebug(true);

    smtpData.setLogin("smtp.gmail.com", 587, "kot.zakhar@gmail.com", "arpskkthwnjqijdd");
    smtpData.setSender("Zakhar", "kot.zakhar@gmail.com");
    smtpData.setPriority("High");
    // smtpData.setSubject(subject.c_str());
    // smtpData.setMessage(message.c_str(), isHtml);
    // smtpData.addRecipient(recipient.c_str());
    
    //Set the subject
    smtpData.setSubject("ESP32 SMTP Mail Sending Test");

    //Set the message - normal text or html format
    smtpData.setMessage("<div style=\"color:#ff0000;font-size:20px;\">Hello World! - From ESP32</div>", true);

    //Add recipients, can add more than one recipient
    smtpData.addRecipient("kot.zakhar@gmail.com");

    smtpData.addCustomMessageHeader("Date: Sat, 10 Aug 2019 21:39:56 -0700 (PDT)");
    
    smtpData.setSendCallback(sendCallback);

    bool result = MailClient.sendMail(smtpData);
    if (!result){
        Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    }
    smtpData.empty();
    return result;
}