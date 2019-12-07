#pragma once
#ifndef _MEMORY_CONTROLLER_H_
#define _MEMORY_CONTROLLER_H_

#include <Preferences.h>

#define STRING_LENGTH 50
#define STRING_MESSAGE_LENGTH 150

#define SMTP_SERVER 0
#define SMTP_PORT 1
#define SMTP_LOGIN 2
#define SMTP_PASS 3
#define SMTP_SENDER 4
#define SMTP_RECIPIENT 5
#define SMTP_SETTINGS_COUNT 6

#define CONFIG_IS_DEFAULT true

extern const char* smtp_settings[];

void InitMemoryController();

int SaveWiFiCredentialsInMemory(const char* ssid, const char* password);

void ClearWiFiCredentials();

int GetWiFiCredentialsAmountFromMemory();
char* GetWiFiSsidFromMemory(int index, char* buffer);
char* GetWiFiPasswordFromMemory(int index, char* buffer);

bool IsConfigStateInMemory();
void SetStateInMemory(bool debug = CONFIG_IS_DEFAULT);

bool SmtpValuesAvailable();
char* GetSmtpValue(int key, char* buffer);
void SetSmtpValue(int key, const char* buffer);

#endif