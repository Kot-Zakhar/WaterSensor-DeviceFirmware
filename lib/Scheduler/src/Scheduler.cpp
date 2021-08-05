#include "Scheduler.h"
#include <Ticker.h>
#include <CommandHandlers.h>

void restartEvent() {
    esp_restart();
}

void sendTestSmsEvent() {
    sendTestSms();
}

void sendTestEmailWiFiEvent() {
    sendTestEmailWiFi();
}

void sendTestEmailGprsEvent() {
    sendTestEmailGprs();
}

Ticker restartScheduler(restartEvent, 0, 1);
Ticker smsScheduler(sendTestSmsEvent, 0, 1);
Ticker emailWiFiScheduler(sendTestEmailWiFiEvent, 0, 1);
Ticker emailGprsScheduler(sendTestEmailGprsEvent, 0, 1);

void processScheduler() {
    restartScheduler.update();
    smsScheduler.update();
    emailWiFiScheduler.update();
    emailGprsScheduler.update();
}

void scheduleRestart(uint32_t delay) {
    restartScheduler.interval(delay);
    restartScheduler.start();
}

void scheduleTestSms() {
    smsScheduler.start();
}

void scheduleTestEmailWiFi() {
    emailWiFiScheduler.start();
}

void scheduleTestEmailGprs() {
    emailGprsScheduler.start();
}
