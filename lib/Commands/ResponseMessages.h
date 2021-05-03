#ifndef _RESPONSE_MESSAGES_H_
#define _RESPONSE_MESSAGES_H_

static const char *status_ok_message = "OK";
static const char *status_error_message = "ERROR";

static const char *payload_not_valid_error_message = "Payload is not valid.";

static const char *wifi_ssid_request_message = "Provide ssid of network:";
static const char *wifi_ssid_confirmation_message = "Ssid received.";
static const char *wifi_password_request_message = "Provide passowrd of network:";
static const char *wifi_password_confirmation_message = "Password received.";
static const char *erasing_wifi_credentials_message = "Erasing wifi credentials.";
static const char *specify_wifi_index_or_uuid_message = "Specify index or uuid.";
static const char *unknown_command_message = "Unknown command.";
static const char *not_supported_command_message = "Not supported.";
static const char *restart_message = "Restarting...";
static const char *pong_message = "pong";
static const char *memory_empty_message = "No networks in memory";

#endif