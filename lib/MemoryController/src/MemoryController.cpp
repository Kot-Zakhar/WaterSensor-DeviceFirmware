#include <MemoryController.h>

#define MEMORY_KEY_MAX_LENGTH 15

// memory keys
const char* wifi_table_name = "bt_wifi";
const char* wifi_ssid_key_prefix = "wf_ssid";
const char* wifi_password_key_prefix = "wf_pass";
const char* wifi_amount_key = "wf_amount";

const char *email_key_prefix = "@";

const char* email_settings[EMAIL_SETTINGS_COUNT] = {
  "smtp server", 
  "smtp port", 
  "sender", 
  "recipient",
  "imap server",
  "imap port",
  "login", 
  "pass", 
};

const char* state_is_config_key = "dbg_state";

Preferences memory;

void InitMemoryController(){
  memory.begin(wifi_table_name, false);

  if (!EmailValuesAvailable()){
    SetEmailValue(EMAIL_SMTP_SERVER, PERSONAL_SMTP_SERVER);
    SetEmailValue(EMAIL_SMTP_PORT, PERSONAL_SMTP_PORT);
    SetEmailValue(EMAIL_IMAP_SERVER, PERSONAL_IMAP_SERVER);
    SetEmailValue(EMAIL_IMAP_PORT, PERSONAL_IMAP_PORT);
    SetEmailValue(EMAIL_LOGIN, PERSONAL_EMAIL_LOGIN);
    SetEmailValue(EMAIL_PASS, PERSONAL_EMAIL_PASSWORD);
    SetEmailValue(EMAIL_SMTP_SENDER, "ESP32");
    SetEmailValue(EMAIL_SMTP_RECIPIENT, PERSONAL_EMAIL_LOGIN);
  }
}

// email and smtp credentials

bool EmailValuesAvailable(){
  log_v("Checking smtp values.");
  char *buffer = (char *)malloc(STRING_LENGTH);
  bool result = true;
  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++){
    if (memory.getString((String(email_key_prefix) + email_settings[i]).c_str(), buffer, STRING_LENGTH) == 0){
      // log_v("Value %d not available.", i);
      result = false;
      break;
    } else {
      // log_v("Value %d: %s", i, buffer);
    }
  }

  if (result)
    log_v("Values are checked.");
  else
    log_v("Values are not available.");

  free(buffer);
  return result;
}

char* GetEmailValue(int key, char* buffer){
  memory.getString(
    (String(email_key_prefix) + email_settings[key]).c_str(),
    buffer,
    STRING_LENGTH
  );
  return buffer;
}

void SetEmailValue(int key, const char* buffer){
  memory.putString(
    (String(email_key_prefix) + email_settings[key]).c_str(),
    buffer
  );
}

// WiFi credentials

int SaveWiFiCredentialsInMemory(const char* ssid, const char* password){
  char* ssid_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char* password_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));

  int counter = GetWiFiCredentialsAmountFromMemory();
  
  sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, counter);
  sprintf(password_key, "%s%d", wifi_password_key_prefix, counter);

  memory.putString(ssid_key, ssid);
  memory.putString(password_key, password);

  memory.putUInt(wifi_amount_key, ++counter);

  Serial.println("saved credantials");
  Serial.println(ssid);
  Serial.println(password);

  free(ssid_key);
  free(password_key);

  return counter;
}

int GetWiFiCredentialsAmountFromMemory(){
  return memory.getUInt(wifi_amount_key, 0);
}

char* GetWiFiSsidFromMemory(int index, char* buffer){
  char* ssid_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, index);
  memory.getString(ssid_key, buffer, STRING_LENGTH);
  free(ssid_key);
  return buffer;
}

char* GetWiFiPasswordFromMemory(int index, char* buffer){
  char* password_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  sprintf(password_key, "%s%d", wifi_password_key_prefix, index);
  memory.getString(password_key, buffer, STRING_LENGTH);
  free(password_key);
  return buffer;
}

int RemoveWiFiCredentials(const char *uuid) {
  int amount = GetWiFiCredentialsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char *key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

  for (int i = 0; i < amount;) {
    sprintf(key, "%s%d", wifi_ssid_key_prefix, i);
    memory.getString(key, buffer, STRING_LENGTH);

    if (!strcmp(uuid, buffer) && RemoveWiFiCredentials(i)) {
      removedAmount++;
      amount--;
    } else {
      i++;
    }
  }

  log_d("%d records of '%s' were removed from memory.", removedAmount, uuid);
  
  free(buffer);
  free(key);

  return removedAmount;
}

bool RemoveWiFiCredentials(int index) {
  int amount = GetWiFiCredentialsAmountFromMemory();
  if (index >= amount || index < 0)
    return false;

  char *key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

  for (int i = index + 1; i < amount; i++) {
    sprintf(key, "%s%d", wifi_ssid_key_prefix, i);
    memory.getString(key, buffer, STRING_LENGTH);
    sprintf(key, "%s%d", wifi_ssid_key_prefix, i - 1);
    memory.putString(key, buffer);

    sprintf(key, "%s%d", wifi_password_key_prefix, i);
    memory.getString(key, buffer, STRING_LENGTH);
    sprintf(key, "%s%d", wifi_password_key_prefix, i - 1);
    memory.putString(key, buffer);

  }

  sprintf(key, "%s%d", wifi_ssid_key_prefix, amount - 1);
  memory.remove(key);
  sprintf(key, "%s%d", wifi_password_key_prefix, amount - 1);
  memory.remove(key);
  
  amount = amount - 1;

  memory.putUInt(wifi_amount_key, amount);

  free(buffer);
  free(key);

  return true;
}

void RemoveAllWiFiCredentials(){
  int amount = GetWiFiCredentialsAmountFromMemory();
  char* key = (char*)malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  
  for (int i = 0; i < amount; i++){
    sprintf(key, "%s%d", wifi_ssid_key_prefix, i);
    memory.remove(key);
    sprintf(key, "%s%d", wifi_password_key_prefix, i);
    memory.remove(key);
  }

  memory.remove(wifi_amount_key);
    
  free(key);
}

// States (modes)

bool IsConfigStateInMemory(){
  log_v("Getting state from memory");
  return memory.getBool(state_is_config_key, CONFIG_IS_DEFAULT);
}

void SetStateInMemory(bool debug) {
  memory.putBool(state_is_config_key, debug);
}