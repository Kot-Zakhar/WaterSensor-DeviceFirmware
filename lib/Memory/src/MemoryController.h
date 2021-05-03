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
#include <custom_defaults.h>

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

int saveWiFiCredentialsToMemory(const char* ssid, const char* password);

void removeAllWiFiCredentialsFromMemory();
bool removeWiFiCredentialsFromMemory(int index);
int removeWiFiCredentialsFromMemory(const char *uuid);

int getWiFiCredentialsAmountFromMemory();
char* getWiFiSsidFromMemory(int index, char* buffer);
char* getWiFiPasswordFromMemory(int index, char* buffer);

bool isConfigStateInMemory();
void setStateInMemory(bool debug = CONFIG_IS_DEFAULT);

bool emailValuesAvailable();
char* getEmailValueFromMemory(int key, char* buffer);
void setEmailValueToMemory(int key, const char* buffer);

int saveEmailRecipientToMemory(const char *email);
bool removeEmailRecipientFromMemory(int index);
int removeEmailRecipientFromMemory(const char *email);
int getEmailRecipientsAmountFromMemory();
char *getEmailRecipientFromMemory(int index, char *buffer);

int savePhoneRecipientToMemory(const char *phone);
bool removePhoneRecipientFromMemory(int index);
int removePhoneRecipientFromMemory(const char *phone);
int getPhoneRecipientsAmountFromMemory();
char *getPhoneRecipientFromMemory(int index, char *buffer);

#endif