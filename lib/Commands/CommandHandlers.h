#ifndef _COMMAND_HANDLERS_H_
#define _COMMAND_HANDLERS_H_

#include <custom_defaults.h>
#include <custom_types.h>

struct WiFiCred *wifiCredsGetAll(struct WiFiCred* credsBuffer, int &amount);
int wifiCredsGetAmount();
void wifiCredsAdd(struct WiFiCred creds);
void wifiCredsAdd(const char *ssid, const char *password);

// char *pingCommandHandler(char *responseBuffer, size_t len);

// void wifiCredsGetCommandHandler(int payload);
// void wifiCredsAddCommandHandler(int payload);
// void wifiCredsDeleteSingleCommandHandler(int payload);
// void wifiCredsDeleteAllCommandHandler(int payload);

// void smtpSettingsGetCommandHandler(int payload);
// void smtpSettingsSetCommandHandler(int payload);

// void imapSettingsGetCommandHandler(int payload);
// void imapSettingsSetCommandHandler(int payload);

// void emailRecipientsGetCommandHandler(int payload);
// void emailRecipientsAddCommandHandler(int payload);
// void emailRecipientDeleteSingleCommandHandler(int payload);
// void emailRecipientsDeleteAllCommandHandler(int payload);

// void gsmPinSetCommandHandler(int payload);
// void gsmUseNetworkPermissionGetCommandHandler(int payload);
// void gsmUseNetworkPermissionSetCommandHandler(int payload);
// void gsmRecipientsGetCommandHandler(int payload);
// void gsmRecipientsAddCommandHandler(int payload);
// void gsmRecipientsDeleteSingleCommandHandler(int payload);
// void gsmRecipientsDeleteAllCommandHandler(int payload);

#endif