#ifndef _COMMANDS_H_
#define _COMMANDS_H_

enum command_t {
  NOT_AVAILABLE = -2,
  NOT_RECOGNISED,

  PING_COMMAND,

  WIFI_CREDS_COMMAND,

  EMAIL_SMTP_COMMAND,
  EMAIL_IMAP_COMMAND,
  EMAIL_RECIPIENTS_COMMAND,

  GSM_PIN_COMMAND,
  GSM_USE_NETWORK_PERMISSION_COMMAND,
  GSM_RECIPIENTS_COMMAND,

  SENSOR_VALUE_COMMAND,
  SENSOR_BOUNDARIES_COMMAND,

  MODE_COMMAND,
  RESTART_COMMAND,
  HELP_COMMAND,

  COMMAND_AMOUNT,
};

// commands
static const char *commands[] = {
  "ping",

  "wifi-creds",
  
  "email-smtp",
  "email-imap",
  "email-recipients",
  
  "gsm-pin",
  "gsm-use-network-perm",
  "gsm-recipients",

  "sensor-value",
  "sensor-boundaries",

  "mode",
  "restart",
  "help"
};

enum command_method_t {
  NO_METHOD = -1,
  GET,
  POST,
  DELETE,

  METHOD_AMOUNT
};

static const char *commandMethods[] = {
  "get",
  "post",
  "delete"
};

// static const char* commands[] = {
//   "ping",
//   "wifi",
//   "wifi-add",
//   "wifi-delete-all",
//   "wifi-delete-single",

//   "email-smtp",
//   "email-smtp-set",

//   "email-imap",
//   "email-imap-set",

//   "email-recipients",
//   "email-recipients-add",
//   "email-recipients-delete-single",
//   "email-recipients-delete-all",

//   "gsm-pin-set",
//   "gsm-use-network",
//   "gsm-use-network-set",
//   "gsm-recipients",
//   "gsm-recipients-add",
//   "gsm-recipients-delete-single",
//   "gsm-recipients-delete-all",

//   "restart",
//   "switch mode",
//   "get mode",
//   "sensor value",
//   "help",
//   "sms"
// };

#endif