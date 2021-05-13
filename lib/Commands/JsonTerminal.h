#ifndef _JSON_TERMINAL_H_
#define _JSON_TERMINAL_H_

#include <ArduinoJson.h>
#include <custom_types.h>
#include <Commands.h>

char *processJsonMessage(const char *req, size_t reqLen, char *outputBuffer, size_t outputLen, error_t &parsingError);
void processJsonMessage(JsonVariant &reqObj, DynamicJsonDocument &resDoc);


error_t defaultOkEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t defaultErrorEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t pingJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t wifiCredsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t emailSmtpJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t emailImapJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t emailRecipientsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t gsmPinJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t gsmRecipientsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t gprsUseNetworkPermissionJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t gprsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t notificationTestEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t sensorValueJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t sensorBoundariesJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t modeJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

// error_t *restartJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

// error_t *helpJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

// error_t *notRecognizedJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);


static error_t (* jsonEndpoints[COMMAND_AMOUNT + 1])(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) = {
    pingJsonEndpoint,
    wifiCredsJsonEndpoint,

    emailSmtpJsonEndpoint,
    emailImapJsonEndpoint,
    emailRecipientsJsonEndpoint,

    gsmPinJsonEndpoint,
    gsmRecipientsJsonEndpoint,

    gprsUseNetworkPermissionJsonEndpoint,
    gprsJsonEndpoint,

    notificationTestEndpoint,

    sensorValueJsonEndpoint,
    sensorBoundariesJsonEndpoint,

    modeJsonEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultErrorEndpoint,
};

#endif