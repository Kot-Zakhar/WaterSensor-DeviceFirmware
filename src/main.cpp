#include <BluetoothSerial.h>
#include <BtTerminalController.h>
#include <MemoryController.h>

void setup() {
  Serial.begin(115200);


  InitMemoryController();
  InitBtTerminalController();
  WiFiControllerInit();
  PrintNetworksFromMemoryCommand();
}

void loop() {
  ProcessBt();
  delay(20);
}


// #include <Arduino.h>
// #include "ESP32_MailClient.h"

// #define WIFI_SSID "KOTPC"
// #define WIFI_PASSWORD "6V2490a3"


// //The Email Sending data object contains config and data to send
// SMTPData smtpData;

// //Callback function to get the Email sending status
// void sendCallback(SendStatus info);

// void setup()
// {

//   Serial.begin(115200);
//   Serial.println();

//   Serial.print("Connecting to AP");

//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.print(".");
//     delay(200);
//   }

//   Serial.println("");
//   Serial.println("WiFi connected.");
//   Serial.print("IP local: ");
//   Serial.println();


//   //Set the Email host, port, account and password
//   smtpData.setLogin("smtp.gmail.com", 587, "kot.zakhar@gmail.com", "ejdzxmntqtyulksj");

//   //For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
//   //enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
//   // smtpData.setSTARTTLS(true);
  
  

//   //Set the sender name and Email
//   smtpData.setSender("Zakhar", "kot.zakhar@gmail.com");

//   //Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
//   smtpData.setPriority("High");

//   //Set the subject
//   smtpData.setSubject("ESP32 SMTP Mail Sending Test");

//   //Set the message - normal text or html format
//   smtpData.setMessage("<div style=\"color:#ff0000;font-size:20px;\">Hello World! - From ESP32</div>", true);

//   //Add recipients, can add more than one recipient
//   smtpData.addRecipient("kot.zakhar@gmail.com");


//   //Add some custom header to message
//   //See https://tools.ietf.org/html/rfc822
//   //These header fields can be read from raw or source of message when it received)
//   smtpData.addCustomMessageHeader("Date: Sat, 10 Aug 2019 21:39:56 -0700 (PDT)");
//   //Be careful when set Message-ID, it should be unique, otherwise message will not store
//   // smtpData.addCustomMessageHeader("Message-ID: <abcde.fghij@gmail.com>");

//   smtpData.setSendCallback(sendCallback);

//   // MailClient.sendMail(smtpData);

//   // delay(60000);

//   // //Start sending Email, can be set callback function to track the status
//   if (!MailClient.sendMail(smtpData))
//     Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

//   //Clear all data from Email object to free memory
//   smtpData.empty();

// }

// void loop()
// {
// }

// //Callback function to get the Email sending status
// void sendCallback(SendStatus msg)
// {
//   //Print the current status
//   Serial.println(msg.info());

//   //Do something when complete
//   if (msg.success())
//   {
//     Serial.println("----------------");
//   }
// }

