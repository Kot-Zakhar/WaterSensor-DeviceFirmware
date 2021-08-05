#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdint.h>

void scheduleRestart(uint32_t delay);
void processScheduler();
void scheduleTestSms();
void scheduleTestEmailWiFi();
void scheduleTestEmailGprs();

#endif