#include <MemoryController.h>
#include <LITTLEFS.h>

#define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
#define MEMORY_KEY_MAX_LENGTH 15

// memory keys
const char* table_name = "mem";
const char* wifi_ssid_key_prefix = "wf_ssid";
const char* wifi_password_key_prefix = "wf_pass";
const char* wifi_amount_key = "wf_amount";

const char *email_key_prefix = "@";
const char *email_server_keys[] = {
  "smtp",
  "imap"
};
const char *email_server_settings_key_prefixes[] = {
  "srv",
  "port",
  "login",
  "pwd",
  "ssl"
};

const char *gsm_pin_key = "gsmpin";

const char *gprs_key_prefix = "gprs";
const char *gprs_perm_key = "gprs_perm";
const char *gprs_settings_keys[] = {
  "apn",
  "usr",
  "pwd"
};

const char *device_state_key = "st";
const char *preferred_device_config_state_key = "c_st";

const char *email_recipient_key_prefix = "e_cred";
const char *email_recipients_amount_key = "e_amount";
const char *phone_recipient_key_prefix = "ph_cred";
const char *phone_recipients_amount_key = "ph_amount";

Preferences memory;

void initMemoryController(){
  memory.begin(table_name, false);
  LITTLEFS.begin();

  // TODO: initialization with defined values
}

#pragma region  sensors bounds

static int water_low_default = 1000;
static int water_high_default = 2000;

void saveWaterSensorBoundariesToMemory(int low, int high) {
  memory.putInt("ws_l", low);
  memory.putInt("ws_h", high);
  memory.putBool("ws", true);
  log_d("saving water bounds: %d %d", low, high);
}

void getWaterSensorBoundariesFromMemory(int &low, int &high) {
  low = memory.getInt("ws_l", water_low_default);
  high = memory.getInt("ws_h", water_high_default);
}

void deleteWaterSensorBoundariesFroMemory() {
  memory.remove("ws");
  memory.remove("ws_l");
  memory.remove("ws_l");
}

static int temp_low_default = 20;
static int temp_high_default = 50;

void saveTemperatureBoundariesToMemory(int &low, int &high) {
  memory.putInt("ts_l", low);
  memory.putInt("ts_h", high);
  memory.putBool("ts", true);
  log_d("saving temp bounds: %d %d", low, high);
}

void getTemperatureBoundariesFromMemory(int &low, int &high) {
  low = memory.getInt("ts_l", temp_low_default);
  high = memory.getInt("ts_h", temp_high_default);
}

void deleteTemperatureBoundariesFroMemory() {
  memory.remove("ts");
  memory.remove("ts_l");
  memory.remove("ts_h");
}

static int humid_low_default = 20;
static int humid_high_default = 55;

void saveHumidityBoundariesToMemory(int &low, int &high) {
  memory.putInt("hs_l", low);
  memory.putInt("hs_h", high);
  memory.putBool("hs", true);
  log_d("saving temp bounds: %d %d", low, high);
}

void getHumidityBoundariesFromMemory(int &low, int &high) {
  low = memory.getInt("hs_l", humid_low_default);
  high = memory.getInt("hs_h", humid_high_default);
}

void deleteHumidityBoundariesFroMemory() {
  memory.remove("hs");
  memory.remove("hs_l");
  memory.remove("hs_h");
}

#pragma endregion  sensors bounds

#pragma region  email and smtp credentials

bool emailServerSettingsAvailable(email_server_type_t serverType) {
  return memory.getBool(email_server_keys[serverType]);
}

void saveEmailServerSettingsToMemory(email_server_type_t serverType, struct EmailServerSettings &serverSettings) {
  char key_buffer[STRING_LENGTH];
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_ADDRESS]);
  memory.putString(key_buffer, serverSettings.server);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_PORT]);
  memory.putString(key_buffer, serverSettings.port);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_LOGIN]);
  memory.putString(key_buffer, serverSettings.login);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_PASSWORD]);
  memory.putString(key_buffer, serverSettings.password);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_SSL]);
  memory.putBool(key_buffer, serverSettings.ssl);
  memory.putBool(email_server_keys[serverType], true);
}

void getEmailServerSettingsFromMemory(email_server_type_t serverType, struct EmailServerSettings &serverSettings) {
  if (!emailServerSettingsAvailable(serverType))
    return;
  char key_buffer[STRING_LENGTH];
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_ADDRESS]);
  memory.getString(key_buffer, serverSettings.server, STRING_LENGTH);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_PORT]);
  memory.getString(key_buffer, serverSettings.port, STRING_LENGTH);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_LOGIN]);
  memory.getString(key_buffer, serverSettings.login, STRING_LENGTH);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_PASSWORD]);
  memory.getString(key_buffer, serverSettings.password, STRING_LENGTH);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_SSL]);
  serverSettings.ssl = memory.getBool(key_buffer);
}

void removeEmailServerSettingsFromMemory(email_server_type_t serverType) {
  if (!emailServerSettingsAvailable(serverType))
    return;
  char key_buffer[STRING_LENGTH];
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_ADDRESS]);
  memory.remove(key_buffer);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_PORT]);
  memory.remove(key_buffer);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_LOGIN]);
  memory.remove(key_buffer);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_PASSWORD]);
  memory.remove(key_buffer);
  sprintf(key_buffer, "%s_%s", email_server_keys[serverType], email_server_keys[EMAIL_SERVER_SSL]);
  memory.remove(key_buffer);
  memory.putBool(email_server_keys[serverType], false);
}

#pragma endregion  email and smtp credentials

#pragma region  gprs settings

bool gprsSettingsAvailableImMemory() {
  return memory.getBool(gprs_key_prefix);
}

bool getGprsPermFromMemory() {
  return memory.getBool(gprs_perm_key);
}

void saveGprsPermToMemory(bool perm) {
  memory.putBool(gprs_perm_key, perm);
}

void saveGprsSettingsToMemory(GprsSettings &serverSettings) {
  char key_buffer[MEMORY_KEY_MAX_LENGTH];
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_APN]);
  memory.putString(key_buffer, serverSettings.apn);
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_USER]);
  memory.putString(key_buffer, serverSettings.user);
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_PASSWORD]);
  memory.putString(key_buffer, serverSettings.password);
  memory.putBool(gprs_key_prefix, true);
}

void getGprsSettingsFromMemory(GprsSettings &serverSettings) {
  if (!gprsSettingsAvailableImMemory())
    return;
  char key_buffer[MEMORY_KEY_MAX_LENGTH];
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_APN]);
  memory.getString(key_buffer, serverSettings.apn, STRING_LENGTH);
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_USER]);
  memory.getString(key_buffer, serverSettings.user, STRING_LENGTH);
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_PASSWORD]);
  memory.getString(key_buffer, serverSettings.password, STRING_LENGTH);
}

void removeGprsSettingsFromMemory() {
  if (!gprsSettingsAvailableImMemory())
    return;
  memory.putBool(gprs_key_prefix, false);
  char key_buffer[MEMORY_KEY_MAX_LENGTH];
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_APN]);
  memory.remove(key_buffer);
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_USER]);
  memory.remove(key_buffer);
  sprintf(key_buffer, "%s_%s", gprs_key_prefix, gprs_settings_keys[GPRS_PASSWORD]);
  memory.remove(key_buffer);
}

#pragma endregion  gprs settings

#pragma region  WiFi credentials

int saveWiFiCredentialsToMemory(const char* ssid, const char* password){
  char* ssid_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char* password_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));

  int counter = getWiFiCredentialsAmountFromMemory();
  
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

int getWiFiCredentialsAmountFromMemory(){
  return memory.getUInt(wifi_amount_key, 0);
}

char* getWiFiSsidFromMemory(int index, char* buffer){
  char ssid_key[MEMORY_KEY_MAX_LENGTH];
  sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, index);
  memory.getString(ssid_key, buffer, STRING_LENGTH);
  return buffer;
}

char* getWiFiPasswordFromMemory(int index, char* buffer){
  char password_key[MEMORY_KEY_MAX_LENGTH];
  sprintf(password_key, "%s%d", wifi_password_key_prefix, index);
  memory.getString(password_key, buffer, STRING_LENGTH);
  return buffer;
}

int removeWiFiCredentialsFromMemory(const char *ssid, const char *password) {
  int amount = getWiFiCredentialsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char key[MEMORY_KEY_MAX_LENGTH];
  char ssid_buffer[STRING_LENGTH];
  char password_buffer[STRING_LENGTH];

  for (int i = 0; i < amount;) {
    sprintf(key, "%s%d", wifi_ssid_key_prefix, i);
    memory.getString(key, ssid_buffer, STRING_LENGTH);
    sprintf(key, "%s%d", wifi_password_key_prefix, i);
    memory.getString(key, password_buffer, STRING_LENGTH);


    if (!strcmp(ssid, ssid_buffer) && !strcmp(password, password_buffer) && removeWiFiCredentialsFromMemory(i)) {
      removedAmount++;
      amount--;
    } else {
      i++;
    }
  }

  log_d("%d records of '%s' were removed from memory.", removedAmount, ssid);
  
  return removedAmount;
}

int removeWiFiCredentialsFromMemory(const char *ssid) {
  int amount = getWiFiCredentialsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char key[MEMORY_KEY_MAX_LENGTH];
  char buffer[STRING_LENGTH];

  for (int i = 0; i < amount;) {
    sprintf(key, "%s%d", wifi_ssid_key_prefix, i);
    memory.getString(key, buffer, STRING_LENGTH);

    if (!strcmp(ssid, buffer) && removeWiFiCredentialsFromMemory(i)) {
      removedAmount++;
      amount--;
    } else {
      i++;
    }
  }

  log_d("%d records of '%s' were removed from memory.", removedAmount, ssid);
  
  return removedAmount;
}

bool removeWiFiCredentialsFromMemory(int index) {
  int amount = getWiFiCredentialsAmountFromMemory();
  if (index >= amount || index < 0)
    return false;

  char key[MEMORY_KEY_MAX_LENGTH];
  char buffer[STRING_LENGTH];

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

  return true;
}

void removeAllWiFiCredentialsFromMemory(){
  int amount = getWiFiCredentialsAmountFromMemory();
  char key[MEMORY_KEY_MAX_LENGTH];
  
  for (int i = 0; i < amount; i++){
    sprintf(key, "%s%d", wifi_ssid_key_prefix, i);
    memory.remove(key);
    sprintf(key, "%s%d", wifi_password_key_prefix, i);
    memory.remove(key);
  }

  memory.remove(wifi_amount_key);
}

#pragma endregion  WiFi credentials

#pragma region  States (modes)

device_state_t getStateFromMemory() {
  return device_state_t(memory.getChar(device_state_key, DEFAULT_DEVICE_CONFIG_STATE));
  // return DEVICE_STATE_CONFIG_WIFI_HOTSPOT;
}

device_state_t getPreferredConfigStateFromMemory() {
  return device_state_t(memory.getChar(preferred_device_config_state_key, DEFAULT_DEVICE_CONFIG_STATE));
}

void setStateInMemory(device_state_t state) {
  log_d("Saving state in memory: %d", state);
  memory.putChar(device_state_key, state);
}

void setPreferredConfigStateFromMemory(device_state_t configState) {
  char state = configState & DEVICE_STATE_CONFIG ? configState : DEFAULT_DEVICE_CONFIG_STATE;
  memory.putChar(preferred_device_config_state_key, state);
}

#pragma endregion  States (modes)

#pragma region  Email recipients

int saveEmailRecipientToMemory(const char *email) {
  char email_key[MEMORY_KEY_MAX_LENGTH];

  int counter = getEmailRecipientsAmountFromMemory();
  
  sprintf(email_key, "%s%d", email_recipient_key_prefix, counter);

  memory.putString(email_key, email);

  memory.putUInt(email_recipients_amount_key, ++counter);

  Serial.println("saved email");
  Serial.println(email);

  return counter;
}

bool removeEmailRecipientFromMemory(int index) {
  int amount = getEmailRecipientsAmountFromMemory();
  if (index >= amount || index < 0)
    return false;

  char email_key[MEMORY_KEY_MAX_LENGTH];
  char buffer[STRING_LENGTH];

  for (int i = index + 1; i < amount; i++) {
    sprintf(email_key, "%s%d", email_recipient_key_prefix, i);
    memory.getString(email_key, buffer, STRING_LENGTH);
    sprintf(email_key, "%s%d", email_recipient_key_prefix, i - 1);
    memory.putString(email_key, buffer);
  }
  
  sprintf(email_key, "%s%d", email_recipient_key_prefix, amount - 1);
  memory.remove(email_key);

  amount = amount - 1;
  memory.putUInt(email_recipients_amount_key, amount);

  return true;
}

int removeEmailRecipientFromMemory(const char *email) {
  int amount = getEmailRecipientsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char key[MEMORY_KEY_MAX_LENGTH];
  char buffer[STRING_LENGTH];

  for (int i = 0; i < amount;) {
    sprintf(key, "%s%d", email_recipient_key_prefix, i);
    memory.getString(key, buffer, STRING_LENGTH);

    if (!strcmp(email, buffer) && removeEmailRecipientFromMemory(i)) {
      removedAmount++;
      amount--;
    } else {
      i++;
    }
  }

  log_d("%d records of '%s' were removed from memory.", removedAmount, email);
  
  return removedAmount;
}

int getEmailRecipientsAmountFromMemory() {
  return memory.getUInt(email_recipients_amount_key, 0);
}

void removeAllEmailRecipientsFromMemory() {
  int amount = getEmailRecipientsAmountFromMemory();

  for (int i = 0; i < amount; i++)
    removeEmailRecipientFromMemory(i);
}

char *getEmailRecipientFromMemory(int index, char *buffer, size_t len) {
  char email_key[MEMORY_KEY_MAX_LENGTH];
  sprintf(email_key, "%s%d", email_recipient_key_prefix, index);
  memory.getString(email_key, buffer, len);
  return buffer;
}

#pragma endregion  Email recipients

#pragma region  gsm pin

void saveGsmPinToMemory(const char *pin) {
  memory.putString(gsm_pin_key, pin);
}

char *getGsmPinFromMemory(char *buffer, size_t len) {
  memory.getString(gsm_pin_key, buffer, len);
  return buffer;
}

void removeGsmPinFromMemory() {
  memory.remove(gsm_pin_key);
}

#pragma endregion  gsm pin

#pragma region  Phone recipients

int saveGsmRecipientToMemory(const char *phone) {
  char phone_key[MEMORY_KEY_MAX_LENGTH];

  int counter = getGsmRecipientsAmountFromMemory();
  
  sprintf(phone_key, "%s%d", phone_recipient_key_prefix, counter);

  memory.putString(phone_key, phone);

  memory.putUInt(phone_recipients_amount_key, ++counter);

  Serial.println("saved phone");
  Serial.println(phone);

  return counter;
}

bool removeGsmRecipientFromMemory(int index) {
  int amount = getGsmRecipientsAmountFromMemory();
  if (index >= amount || index < 0)
    return false;

  char phone_key[MEMORY_KEY_MAX_LENGTH];
  char buffer[STRING_LENGTH];

  for (int i = index + 1; i < amount; i++) {
    sprintf(phone_key, "%s%d", phone_recipient_key_prefix, i);
    memory.getString(phone_key, buffer, STRING_LENGTH);
    sprintf(phone_key, "%s%d", phone_recipient_key_prefix, i - 1);
    memory.putString(phone_key, buffer);
  }
  
  sprintf(phone_key, "%s%d", phone_recipient_key_prefix, amount - 1);
  memory.remove(phone_key);

  amount = amount - 1;
  memory.putUInt(phone_recipients_amount_key, amount);

  return true;
}

int removeGsmRecipientFromMemory(const char *phone) {
  int amount = getGsmRecipientsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char key[MEMORY_KEY_MAX_LENGTH];
  char buffer[STRING_LENGTH];

  for (int i = 0; i < amount;) {
    sprintf(key, "%s%d", phone_recipient_key_prefix, i);
    memory.getString(key, buffer, STRING_LENGTH);

    if (!strcmp(phone, buffer) && removeGsmRecipientFromMemory(i)) {
      removedAmount++;
      amount--;
    } else {
      i++;
    }
  }

  log_d("%d records of '%s' were removed from memory.", removedAmount, phone);
  
  return removedAmount;
}

int getGsmRecipientsAmountFromMemory() {
  return memory.getUInt(phone_recipients_amount_key, 0);
}

void removeAllGsmRecipientsFromMemory() {
  int amount = getGsmRecipientsAmountFromMemory();

  for (int i = 0; i < amount; i++)
    removeGsmRecipientFromMemory(i);
}

char *getGsmRecipientFromMemory(int index, char *buffer, size_t len) {
  char phone_key[MEMORY_KEY_MAX_LENGTH];
  sprintf(phone_key, "%s%d", phone_recipient_key_prefix, index);
  memory.getString(phone_key, buffer, len);
  return buffer;
}

#pragma endregion  Phone recipients