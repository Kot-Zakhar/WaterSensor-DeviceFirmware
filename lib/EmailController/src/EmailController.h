#pragma once
#ifndef _EMAIL_CONTROLLER_H_
#define _EMAIL_CONTROLLER_H_

#include <MemoryController.h>
#include <SensorController.h>
#include <ESP32_MailClient.h>
#include <WifiController.h>

bool SendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess = false);


#endif