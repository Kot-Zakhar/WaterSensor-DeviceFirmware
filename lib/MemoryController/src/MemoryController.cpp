#include <MemoryController.h>

#define MEMORY_KEY_MAX_LENGTH 15

static const char *memoryTag = "MemoryController";

// memory keys
const char* wifi_table_name = "bt_wifi";
const char* wifi_ssid_key_prefix = "wf_ssid";
const char* wifi_password_key_prefix = "wf_pass";
const char* wifi_amount_key = "wf_amount";

const char *smtp_key_prefix = "smtp";

const char* smtp_settings[SMTP_SETTINGS_COUNT] = {
  "server", 
  "port", 
  "login", 
  "pass", 
  "sender", 
  "recipient"
};

const char* state_is_config_key = "dbg_state";

Preferences memory;

void InitMemoryController(){
  memory.begin(wifi_table_name, false);

  SetStateInMemory(IsConfigStateInMemory());
  if (!SmtpValuesAvailable()){
    SetSmtpValue(SMTP_SERVER, PERSONAL_SMTP_SERVER);
    SetSmtpValue(SMTP_PORT, PERSONAL_SMTP_PORT);
    SetSmtpValue(SMTP_LOGIN, PERSONAL_SMTP_LOGIN);
    SetSmtpValue(SMTP_PASS, PERSONAL_SMTP_PASSWORD);
    SetSmtpValue(SMTP_SENDER, "ESP32");
    SetSmtpValue(SMTP_RECIPIENT, PERSONAL_SMTP_LOGIN);
  }
}

// email and smtp credentials

bool SmtpValuesAvailable(){
  log_v("[%s] %s", memoryTag, "Checking smtp values.");
  char *buffer = (char *)malloc(STRING_LENGTH);
  bool result = true;
  for (int i = 0; i < SMTP_SETTINGS_COUNT; i++){
    if (memory.getString((String(smtp_key_prefix) + smtp_settings[i]).c_str(), buffer, STRING_LENGTH) == 0){
      log_v("[%s] %s", memoryTag, "Value %d not available.", i);
      result = false;
      break;
    } else {
      log_v("[%s] %s", memoryTag, "Value %d: %s", i, buffer);
    }
  }

  if (result)
    log_v("[%s] %s", memoryTag, "Values are checked.");
  else
    log_v("[%s] %s", memoryTag, "Values are not available.");

  free(buffer);
  return result;
}

char* GetSmtpValue(int key, char* buffer){
  memory.getString(
    (String(smtp_key_prefix) + smtp_settings[key]).c_str(),
    buffer,
    STRING_LENGTH
  );
  return buffer;
}

void SetSmtpValue(int key, const char* buffer){
  memory.putString(
    (String(smtp_key_prefix) + smtp_settings[key]).c_str(),
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

void ClearMemory(){
    memory.clear();
}

// States (modes)

bool IsConfigStateInMemory(){
  return memory.getBool(state_is_config_key, CONFIG_IS_DEFAULT);
}

void SetStateInMemory(bool debug) {
  memory.putBool(state_is_config_key, debug);
}