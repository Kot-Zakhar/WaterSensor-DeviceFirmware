#ifndef _EMAIL_CONTROLLER_H_
#define _EMAIL_CONTROLLER_H_

#include <MemoryController.h>
#include <SensorController.h>
#include <ESP32_MailClient.h>
#include <WifiController.h>
#include <Ticker.h>
#include <IOController.h>

bool sendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess = false);
void initEmailController();
void stopEmailChecker();
void restartEmailChecker();
bool shouldEmailBeProcessed();
bool checkForIncomingLetter();
void processEmailController();

#endif