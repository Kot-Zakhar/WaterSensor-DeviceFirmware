#ifndef _MEMORY_CONTROLLER_H_
#define _MEMORY_CONTROLLER_H_

#include <Preferences.h>

/* this header file includes macroses with personal info:
    PERSONAL_EMAIL_LOGIN
    PERSONAL_EMAIL_PASSWORD
    PERSONAL_SMTP_SERVER
    PERSONAL_SMTP_PORT
    PERSONAL_IMAP_SERVER
    PERSONAL_IMAP_PORT
*/
#include "personal_info.h"

#define STRING_LENGTH 50
#define STRING_MESSAGE_LENGTH 150
#define STRING_JSON_LENGTH 200

enum email_setting_t {
  EMAIL_SMTP_SERVER,
  EMAIL_SMTP_PORT,
  EMAIL_SMTP_SENDER,
  EMAIL_SMTP_RECIPIENT,
  EMAIL_IMAP_SERVER,
  EMAIL_IMAP_PORT,
  EMAIL_LOGIN,
  EMAIL_PASS,
  EMAIL_SETTINGS_COUNT,
};

#define CONFIG_IS_DEFAULT true

extern const char* email_settings[];

void initMemoryController();

int SaveWiFiCredentialsInMemory(const char* ssid, const char* password);

void RemoveAllWiFiCredentials();
bool RemoveWiFiCredentials(int index);
int RemoveWiFiCredentials(const char *uuid);

int getWiFiCredentialsAmountFromMemory();
char* GetWiFiSsidFromMemory(int index, char* buffer);
char* GetWiFiPasswordFromMemory(int index, char* buffer);

bool isConfigStateInMemory();
void setStateInMemory(bool debug = CONFIG_IS_DEFAULT);

bool EmailValuesAvailable();
char* getEmailValue(int key, char* buffer);
void SetEmailValue(int key, const char* buffer);

#endif