#ifndef _SIM_CONTROLLER_H_
#define _SIM_CONTROLLER_H_

void initGsmController();
void pipeSimToSerial();
void sendSMS(const char* text, const char* mobileNumber);
void sendToAT(const char* command);
void readFromAT(char* buffer, int length);

#endif