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
#include <custom_types.h>

// enum email_setting_t {
//   EMAIL_SMTP_SERVER,
//   EMAIL_SMTP_PORT,
//   EMAIL_SMTP_SENDER,
//   EMAIL_SMTP_RECIPIENT,
//   EMAIL_IMAP_SERVER,
//   EMAIL_IMAP_PORT,
//   EMAIL_LOGIN,
//   EMAIL_PASS,
//   EMAIL_SETTINGS_COUNT,
// };

#define DEFAULT_DEVICE_CONFIG_STATE DEVICE_STATE_CONFIG_BLUETOOTH

extern const char* email_settings[];

void initMemoryController();

device_state_t getStateFromMemory();
void setStateInMemory(device_state_t state);
device_state_t getPreferredConfigStateFromMemory();
void setPreferredConfigStateFromMemory(device_state_t configState);

void saveWaterSensorBoundariesToMemory(int low, int high);
void getWaterSensorBoundariesFromMemory(int &low, int &high);
void deleteWaterSensorBoundariesFroMemory();
void saveTemperatureBoundariesToMemory(int &low, int &high);
void getTemperatureBoundariesFromMemory(int &low, int &high);
void deleteTemperatureBoundariesFroMemory();
void saveHumidityBoundariesToMemory(int &low, int &high);
void getHumidityBoundariesFromMemory(int &low, int &high);
void deleteHumidityBoundariesFroMemory();

int saveWiFiCredentialsToMemory(const char* ssid, const char* password);
void removeAllWiFiCredentialsFromMemory();
bool removeWiFiCredentialsFromMemory(int index);
int removeWiFiCredentialsFromMemory(const char *ssid, const char *password);
int removeWiFiCredentialsFromMemory(const char *ssid);
int getWiFiCredentialsAmountFromMemory();
char* getWiFiSsidFromMemory(int index, char* buffer);
char* getWiFiPasswordFromMemory(int index, char* buffer);

bool emailServerSettingsAvailable(email_server_type_t serverType);
void saveEmailServerSettingsToMemory(email_server_type_t serverType, EmailServerSettings &serverSettings);
void getEmailServerSettingsFromMemory(email_server_type_t serverType, EmailServerSettings &serverSettings);
void removeEmailServerSettingsFromMemory(email_server_type_t serverType);

bool gprsSettingsAvailableImMemory();
bool getGprsPermFromMemory();
void saveGprsPermToMemory(bool perm);
void saveGprsSettingsToMemory(GprsSettings &serverSettings);
void getGprsSettingsFromMemory(GprsSettings &serverSettings);
void removeGprsSettingsFromMemory();

int saveEmailRecipientToMemory(const char *email);
bool removeEmailRecipientFromMemory(int index);
int removeEmailRecipientFromMemory(const char *email);
void removeAllEmailRecipientsFromMemory();
int getEmailRecipientsAmountFromMemory();
char *getEmailRecipientFromMemory(int index, char *buffer, size_t len);

int saveGsmRecipientToMemory(const char *phone);
bool removeGsmRecipientFromMemory(int index);
int removeGsmRecipientFromMemory(const char *phone);
void removeAllGsmRecipientsFromMemory();
int getGsmRecipientsAmountFromMemory();
char *getGsmRecipientFromMemory(int index, char *buffer, size_t len);

void saveGsmPinToMemory(const char *pin);
char *getGsmPinFromMemory(char *buffer, size_t len);
void removeGsmPinFromMemory();

#endif