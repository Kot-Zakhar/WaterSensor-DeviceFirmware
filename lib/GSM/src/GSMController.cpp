#include "GSMController.h"
#include <stdio.h>
#include <HardwareSerial.h>
#include <BtIO.h>

#define MIK_PIN GPIO25
#define SPK_PIN GPIO26

#define SIM_RX GPIO_NUM_39
#define SIM_TX GPIO_NUM_16

String pin = "1604";

HardwareSerial SimSerial(1);

void initGsmController() {
    SimSerial.begin(115200, SERIAL_8N1, SIM_RX, SIM_TX);
    SimSerial.println("AT+CMEE=2");
    delay(100);
    SimSerial.printf("AT+CPIN=%s\n", pin);
    delay(100);
    SimSerial.printf("AT+CLIP=0\n");
    delay(100);
    SimSerial.println("AT+CSCS= \"GSM\"");
    delay(100);
    // String response = SimSerial.readString();
    // response.trim();
    // Serial.printf("Settings ping:%s\n", response.c_str());
}

void pipeSimToSerial() {
    while (SimSerial.available()) {
        String message = SimSerial.readString();
        Serial.printf("pipe: %s", message);
        writeBt("pipe: ");
        writeBt(message.c_str());
    }
}

void sendSMS(const char* text, const char* mobileNumber) {
    SimSerial.println("AT+CMGF=1");
    delay(100);
    SimSerial.printf("AT+CMGS= \"%s\"\n", mobileNumber);
    delay(100);
    SimSerial.readString();
    SimSerial.printf("%s", text);
    SimSerial.write(26);
    delay(100);
    SimSerial.readStringUntil('\n');
    Serial.printf("Sending text: \"%s\"\n", text);
}

void sendToAT(const char* command) {
    SimSerial.print(command);
}

void readFromAT(char* buffer, int length) {
    SimSerial.readBytes((uint8_t *)buffer, length);
}