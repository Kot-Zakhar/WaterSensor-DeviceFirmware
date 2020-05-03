#ifndef BLUETOOTH_COMMANDS_H
#define BLUETOOTH_COMMANDS_H


enum bt_command_t {
  NOT_AVAILABLE = -2,
  NOT_RECOGNISED,

  BT_PING,
  WIFI_SSID_AND_PASSWORD,
  WIFI_ERASE,
  WIFI_SHOW_NETWORKS,
  SMTP_SETTINGS,
  RESTART,
  SWITCH_MODE,
  HELP,

  COMMAND_AMOUNT,

  JSON_BT_COMMANDS = 98,
  JSON_NOT_RECOGNISED,

  JSON_BT_COMMAND = 100,
  JSON_BT_PING = 100,
  JSON_WIFI_SSID_AND_PASSWORD,
  JSON_WIFI_ERASE,
  JSON_WIFI_SHOW_NETWORKS,
  JSON_SMTP_SETTINGS,
  JSON_RESTART,
  JSON_SWITCH_MODE,
  JSON_HELP,

};

// commands
static const char* commands[] = {
  "ping",
  "new wifi",
  "clear wifi",
  "show",
  "email",
  "restart",
  "switch mode",
  "help"
};

// bt messages
static const char *wifi_ssid_request_message = "Provide ssid of network:";
static const char *wifi_ssid_confirmation_message = "Ssid received.";
static const char *wifi_password_request_message = "Provide passowrd of network:";
static const char *wifi_password_confirmation_message = "Password received.";
static const char *erasing_wifi_credentials_message = "Erasing wifi credentials.";
static const char *status_ok_message = "Ok.";
static const char *status_error_message = "Error.";
static const char *unknown_command_message = "Unknown command.";
static const char *restart_message = "Restarting...";
static const char *pong_message = "pong";
static const char *memory_empty_message = "No networks in memory";


static const char* json_prefix = "json:";

#endif