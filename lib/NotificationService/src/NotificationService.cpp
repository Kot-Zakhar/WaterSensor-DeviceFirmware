#include "NotificationService.h"

#include <stdio.h>
#include <string.h>
#include <esp_log.h>

#include <GsmService.h>
#include <EmailService.h>
#include <WifiController.h>

#include <SensorsService.h>
#include <LedController.h>

const char *testMessage = "This is a test message from ESP32.";
const char *alertMessage = "One or several of sensors values have gone out of the bounds.";
const char *valuesFormat = "Water sensor: %d.\nTemperature: %.1f*C.\nHumidity: %.1f%%.";
const char *shortFormat = "water: %d, temp: %.1f*C, hum: %.1f%%";

void notifyAboutEvent(notification_event_t event) {
    redLedOn();
    struct SensorsValues values;
    getCurrentSensorsValues(values);

    char header[50];
    char message[150];

    // prep message
    switch (event)
    {
    case TEST_SMS_NOTIFICATION:
    case TEST_EMAIL_GPRS_NOTIFICATION:
    case TEST_EMAIL_WIFI_NOTIFICATION:
    {
        strlcpy(header, "Test message via GPRS", 50);
        int len = strlcpy(message, testMessage, 150);
        message[len]='\n';
        sprintf(message + len + 1, valuesFormat, values.water, values.temperature, values.humidity);
        break;
    }
    case SENSORS_OUT_OF_BOUNDS_EVENT:
    {
        strlcpy(header, "Alert from ESP32", 50);
        int len = strlcpy(message, alertMessage, 150);
        message[len]='\n';
        sprintf(message + len + 1, valuesFormat, values.water, values.temperature, values.humidity);
        break;
    }
    default:
        break;
    }

    // send message
    switch (event)
    {
    case TEST_EMAIL_GPRS_NOTIFICATION:
        sendEmailGPRS(header, message);
        break;
    case TEST_SMS_NOTIFICATION:
        sendSMS(message);
        break;
    case TEST_EMAIL_WIFI_NOTIFICATION:
        sendLetter("Test message", message, false);
        break;
    case SENSORS_OUT_OF_BOUNDS_EVENT:
        if (GsmNotificationIsOn()) {
            sendSMS(message);
        }
        if (GprsNotificationIsOn()) {
            sendEmailGPRS(header, message);
        } else if (WiFiNotificationIsOn()) {
            bool wasConnected = isWiFiConnected();
            bool connected = wasConnected || connectToAnyWiFiFromMemory();
            if (connected) {
                if (emailNotificationIsON())
                    sendLetter(header, message, false);
                sprintf(message, shortFormat, values.water, values.temperature, values.humidity);
                sendIFTTTRequest(message);
            }
            if (!wasConnected)
                disconnectFromWiFi();
        }
        break;
    default:
        log_d("Not sending message: %s", message);
        break;
    }
    redLedOff();
}