#ifndef BLUETOOTH_COMMANDS_H
#define BLUETOOTH_COMMANDS_H


enum bt_command_t {
  NOT_AVAILABLE = -2,
  NOT_RECOGNISED,

  BT_PING,
  WIFI_SSID_AND_PASSWORD,
  WIFI_REMOVE_ALL,
  WIFI_REMOVE_SINGLE,
  WIFI_SHOW_NETWORKS,
  SMTP_SETTINGS,
  SET_SMTP_SETTINGS,
  RESTART,
  SWITCH_MODE,
  GET_MODE,
  HELP,

  COMMAND_AMOUNT,
};

// commands
static const char* commands[] = {
  "ping",
  "new wifi",
  "remove wifi all",
  "remove wifi single",
  "show",
  "email",
  "set email",
  "restart",
  "switch mode",
  "get mode",
  "help"
};

// bt messages
static const char *wifi_ssid_request_message = "Provide ssid of network:";
static const char *wifi_ssid_confirmation_message = "Ssid received.";
static const char *wifi_password_request_message = "Provide passowrd of network:";
static const char *wifi_password_confirmation_message = "Password received.";
static const char *erasing_wifi_credentials_message = "Erasing wifi credentials.";
static const char *specify_wifi_index_or_uuid_message = "Specify index or uuid.";
static const char *status_ok_message = "Ok.";
static const char *status_error_message = "Error.";
static const char *unknown_command_message = "Unknown command.";
static const char *not_supported_command_message = "Not supported.";
static const char *restart_message = "Restarting...";
static const char *pong_message = "pong";
static const char *memory_empty_message = "No networks in memory";


static const char* json_prefix = "json:";

#endif