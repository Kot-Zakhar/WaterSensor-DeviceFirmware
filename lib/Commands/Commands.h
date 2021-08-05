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
  GSM_RECIPIENTS_COMMAND,

  GPRS_USE_NETWORK_PERMISSION_COMMAND,
  GPRS_COMMAND,

  NOTIFICATION_TEST,

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
  "gsm-recipients",

  "gprs-use-network-perm",
  "gprs",
  
  "notification-test",

  "sensor-values",
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

#endif