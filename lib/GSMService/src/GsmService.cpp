#include "GsmService.h"
#include <stdio.h>
#include <HardwareSerial.h>
#include <BtIO.h>
#include <custom_defaults.h>
#include <MemoryController.h>
#include <StreamDebugger.h>

#define MIK_PIN GPIO25
#define SPK_PIN GPIO26

#define SIM_RX GPIO_NUM_39
#define SIM_TX GPIO_NUM_16

String pin = "1604";

#define LOGGED_SIM_SERIAL

#ifdef LOGGED_SIM_SERIAL
HardwareSerial RealSimSerial(1);
StreamDebugger SimSerial(RealSimSerial, Serial);
#else
HardwareSerial SimSerial(1);
#endif

void initGsmService() {
    #ifdef LOGGED_SIM_SERIAL
    RealSimSerial.begin(115200, SERIAL_8N1, SIM_RX, SIM_TX);
    RealSimSerial.setTimeout(1000);
    #else
    SimSerial.begin(115200, SERIAL_8N1, SIM_RX, SIM_TX);
    SimSerial.setTimeout(1000);
    #endif

    SimSerial.println("ATE0");
    delay(10);
    SimSerial.readString();

    // Errors with description
    SimSerial.println("AT+CMEE=2");
    delay(10);
    SimSerial.readString();

    // SimSerial.printf("AT+CPIN=%s\n", pin);
    // delay(10);
    // SimSerial.readString();

    // no phone calls
    SimSerial.printf("AT+CLIP=0\n");
    delay(10);
    SimSerial.readString();

    // set charset
    SimSerial.println("AT+CSCS=\"GSM\"");
    delay(10);
    SimSerial.readString();
}

bool gsmNeedToBeProcessed() {
    return SimSerial.available();
}

void processGsm() {
    if (SimSerial.available()) {
        String message = SimSerial.readString();
        message.trim();
        Serial.printf("pipe: %s", message);

        if (message.startsWith("+SMTPSEND:")) {
            SimSerial.println("AT+SAPBR=0,1");
            delay(50);
            SimSerial.readString();
        }
    }
}

void sendSMS(const char *text, const char *mobileNumber) {
    SimSerial.println("AT+CMGF=1");
    delay(10);
    SimSerial.printf("AT+CMGS=\"%s\"\n", mobileNumber);
    delay(10);
    SimSerial.readString();
    SimSerial.printf("%s", text);
    SimSerial.write(26);
    delay(10);
    SimSerial.readString();
    Serial.printf("Sending text: \"%s\"\n", text);
}

void sendSMS(const char *text) {
    char *phoneBuffer = (char *)malloc(STRING_LENGTH);
    int amount = getGsmRecipientsAmountFromMemory();

    for (int i = 0; i < amount; i++) {
        phoneBuffer = getGsmRecipientFromMemory(i, phoneBuffer, STRING_LENGTH);
        sendSMS(text, phoneBuffer);
    }

    free(phoneBuffer);
}

void sendEmailGPRS(const char *subject, const char *text) {
    if (!emailServerSettingsAvailable(SMTP_EMAIL_SERVER_TYPE) || !gprsSettingsAvailableImMemory())
        return;

    int rcptAmount = getEmailRecipientsAmountFromMemory();
    if (!rcptAmount)
        return;

    EmailServerSettings smtpSrv;
    getEmailServerSettingsFromMemory(SMTP_EMAIL_SERVER_TYPE, smtpSrv);
    
    GprsSettings gprsSettings;
    getGprsSettingsFromMemory(gprsSettings);

    log_d("Sending letter via gsm to %s %s %s %s %s", smtpSrv.server, smtpSrv.port, smtpSrv.login, smtpSrv.password, smtpSrv.ssl ? "ssl" : "no ssl");
    

    SimSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    delay(50);
    SimSerial.readString();

    SimSerial.printf("AT+SAPBR=3,1,\"APN\",\"%s\"\n", gprsSettings.apn);
    delay(50);
    SimSerial.readString();

    SimSerial.printf("AT+SAPBR=3,1,\"USER\",\"%s\"\n", gprsSettings.user);
    delay(50);
    SimSerial.readString();

    SimSerial.printf("AT+SAPBR=3,1,\"PWD\",\"%s\"\n", gprsSettings.password);
    delay(50);
    SimSerial.readString();

    SimSerial.println("AT+SAPBR=1,1");
    delay(50);
    SimSerial.readString();

    SimSerial.println("AT+SAPBR=2,1");
    delay(50);
    SimSerial.readString();



    SimSerial.printf("AT+EMAILSSL=%d\n", smtpSrv.ssl ? 2: 0);
    delay(50);
    SimSerial.readString();

    SimSerial.println("AT+EMAILCID=1");
    delay(50);
    SimSerial.readString();

    SimSerial.println("AT+EMAILTO=30");
    delay(50);
    SimSerial.readString();

    SimSerial.printf("AT+SMTPSRV=\"%s\",\"%s\"\n", smtpSrv.server, smtpSrv.port);
    delay(50);
    SimSerial.readString();
    
    SimSerial.printf("AT+SMTPAUTH=1,\"%s\",\"%s\"\n", smtpSrv.login, smtpSrv.password);
    delay(50);
    SimSerial.readString();

    SimSerial.printf("AT+SMTPFROM=\"%s\",ESP32\n", smtpSrv.login);
    delay(50);
    SimSerial.readString();

    SimSerial.println("AT+SMTPRCPT?");
    delay(50);
    SimSerial.readString();

    char *recipientBuffer = (char *)malloc(STRING_LENGTH);
    for (int i = 0; i < rcptAmount; i++) {
        getEmailRecipientFromMemory(i, recipientBuffer, STRING_LENGTH);
        SimSerial.printf("AT+SMTPRCPT=0,%d,\"%s\"\n", i, recipientBuffer);
        delay(50);
        SimSerial.readString();
    }
    free(recipientBuffer);

    SimSerial.printf("AT+SMTPSUB=\"%s\"\n", subject);
    delay(50);
    SimSerial.readString();

    int bodyLength = strlen(text);
    SimSerial.printf("AT+SMTPBODY=%d\n", bodyLength);
    delay(50);
    SimSerial.readString();
    
    SimSerial.print(text);
    delay(50);
    SimSerial.readString();

    SimSerial.println("AT+SMTPSEND");
    delay(50);
    SimSerial.readString();

}

bool GsmNotificationIsOn() {
    return getGsmRecipientsAmountFromMemory() > 0;
}

bool GsmConnectionIsAvailable() {
    // TODO: find AT command for checking sms
    return true;
}

bool GprsNotificationIsOn() {
    return gprsSettingsAvailableImMemory() &&
        getGprsPermFromMemory() &&
        getEmailRecipientsAmountFromMemory() > 0; // TODO: or httpNotificationIsOn
}

bool GpssConnectionIsAvailable() {
    // TODO: find AT command to check gprs availability
    return true;
}
