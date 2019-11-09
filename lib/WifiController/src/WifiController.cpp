#include <WifiController.h>
// #include <esp_wifi.h>


const char* host = "api.noopschallenge.com";

void WiFiControllerInit(){
    // esp_event_handler_register()
    // WIFI_EVENT_AP_STACONNECTED

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

String GetDateTime(){
    return "Sat, 10 Nov 2019 21:39:56 -0700 (PDT)";
}

bool SendLetter(String recipient, String subject, String message, bool isHtml){
    SMTPData smtpData;
    smtpData.setLogin("smtp.gmail.com", 587, "kot.zakhar@gmail.com", "brwfdlguftghvapx");
    smtpData.setSTARTTLS(true);
    smtpData.setSender("ESP32", "kot.zakhar@gmail.com");
    smtpData.setPriority("High");
    smtpData.setSubject(subject);
    smtpData.setMessage(message.c_str(), isHtml);
    smtpData.addRecipient(recipient.c_str());
    smtpData.addCustomMessageHeader("Date: " + GetDateTime());
    bool result = MailClient.sendMail(smtpData);
    if (!result){
        Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    }
    return result;
}