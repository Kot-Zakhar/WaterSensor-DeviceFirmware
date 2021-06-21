#ifndef _SIM_CONTROLLER_H_
#define _SIM_CONTROLLER_H_

void initGsmService();

void processGsm();
bool gsmNeedToBeProcessed();

bool GsmNotificationIsOn();
bool GsmConnectionIsAvailable();
bool GprsNotificationIsOn();
bool GprsConnectionIsAvailable();

void sendSMS(const char* text, const char* mobileNumber);
void sendSMS(const char *text);
void sendEmailGPRS(const char *subject, const char *text);
void openServer();
#endif