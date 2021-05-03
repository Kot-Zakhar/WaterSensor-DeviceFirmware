#include <MemoryController.h>

#define MEMORY_KEY_MAX_LENGTH 15

// memory keys
const char* table_name = "bt_wifi";
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

const char *email_recipient_key_prefix = "e_cred";
const char *email_recipients_amount_key = "e_amount";
const char *phone_recipient_key_prefix = "ph_cred";
const char *phone_recipients_amount_key = "ph_amount";

Preferences memory;

void initMemoryController(){
  memory.begin(table_name, false);

  if (!emailValuesAvailable()){
    setEmailValueToMemory(EMAIL_SMTP_SERVER, PERSONAL_SMTP_SERVER);
    setEmailValueToMemory(EMAIL_SMTP_PORT, PERSONAL_SMTP_PORT);
    setEmailValueToMemory(EMAIL_IMAP_SERVER, PERSONAL_IMAP_SERVER);
    setEmailValueToMemory(EMAIL_IMAP_PORT, PERSONAL_IMAP_PORT);
    setEmailValueToMemory(EMAIL_LOGIN, PERSONAL_EMAIL_LOGIN);
    setEmailValueToMemory(EMAIL_PASS, PERSONAL_EMAIL_PASSWORD);
    setEmailValueToMemory(EMAIL_SMTP_SENDER, "ESP32");
    setEmailValueToMemory(EMAIL_SMTP_RECIPIENT, PERSONAL_EMAIL_LOGIN);
  }
}

#pragma region  email and smtp credentials

bool emailValuesAvailable(){
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

char* getEmailValueFromMemory(int key, char* buffer){
  memory.getString(
    (String(email_key_prefix) + email_settings[key]).c_str(),
    buffer,
    STRING_LENGTH
  );
  return buffer;
}

void setEmailValueToMemory(int key, const char* buffer){
  memory.putString(
    (String(email_key_prefix) + email_settings[key]).c_str(),
    buffer
  );
}

#pragma endregion  email and smtp credentials

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
  char* ssid_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, index);
  memory.getString(ssid_key, buffer, STRING_LENGTH);
  free(ssid_key);
  return buffer;
}

char* getWiFiPasswordFromMemory(int index, char* buffer){
  char* password_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  sprintf(password_key, "%s%d", wifi_password_key_prefix, index);
  memory.getString(password_key, buffer, STRING_LENGTH);
  free(password_key);
  return buffer;
}

int removeWiFiCredentialsFromMemory(const char *ssid, const char *password) {
  int amount = getWiFiCredentialsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char *key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *ssid_buffer = (char *) malloc(STRING_LENGTH * sizeof(char));
  char *password_buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

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
  
  free(ssid_buffer);
  free(password_buffer);
  free(key);

  return removedAmount;
}

int removeWiFiCredentialsFromMemory(const char *ssid) {
  int amount = getWiFiCredentialsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char *key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

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
  
  free(buffer);
  free(key);

  return removedAmount;
}

bool removeWiFiCredentialsFromMemory(int index) {
  int amount = getWiFiCredentialsAmountFromMemory();
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

void removeAllWiFiCredentialsFromMemory(){
  int amount = getWiFiCredentialsAmountFromMemory();
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

#pragma endregion  WiFi credentials

#pragma region  States (modes)

bool isConfigStateInMemory(){
  log_v("Getting state from memory");
  return memory.getBool(state_is_config_key, CONFIG_IS_DEFAULT);
}

void setStateInMemory(bool debug) {
  memory.putBool(state_is_config_key, debug);
}

#pragma endregion  States (modes)

#pragma region  Email recipients

int saveEmailRecipientToMemory(const char *email) {

  char* email_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));

  int counter = getEmailRecipientsAmountFromMemory();
  
  sprintf(email_key, "%s%d", email_recipient_key_prefix, counter);

  memory.putString(email_key, email);

  memory.putUInt(email_recipients_amount_key, ++counter);

  Serial.println("saved email");
  Serial.println(email);

  free(email_key);

  return counter;
}

bool removeEmailRecipientFromMemory(int index) {
  int amount = getEmailRecipientsAmountFromMemory();
  if (index >= amount || index < 0)
    return false;

  char* email_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

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

  free(buffer);
  free(email_key);

  return true;
}

int removeEmailRecipientFromMemory(const char *email) {
  int amount = getWiFiCredentialsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char *key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

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
  
  free(buffer);
  free(key);

  return removedAmount;
}

int getEmailRecipientsAmountFromMemory() {
  return memory.getUInt(email_recipients_amount_key, 0);
}

char *getEmailRecipientFromMemory(int index, char *buffer) {
  char* email_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  sprintf(email_key, "%s%d", email_recipient_key_prefix, index);
  memory.getString(email_key, buffer, STRING_LENGTH);
  free(email_key);
  return buffer;
}

#pragma endregion  Email recipients

#pragma region  Phone recipients

int savePhoneRecipientToMemory(const char *phone) {

  char* phone_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));

  int counter = getPhoneRecipientsAmountFromMemory();
  
  sprintf(phone_key, "%s%d", phone_recipient_key_prefix, counter);

  memory.putString(phone_key, phone);

  memory.putUInt(phone_recipients_amount_key, ++counter);

  Serial.println("saved phone");
  Serial.println(phone);

  free(phone_key);

  return counter;
}

bool removePhoneRecipientFromMemory(int index) {
  int amount = getPhoneRecipientsAmountFromMemory();
  if (index >= amount || index < 0)
    return false;

  char* phone_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

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

  free(buffer);
  free(phone_key);

  return true;
}

int removePhoneRecipientFromMemory(const char *phone) {
  int amount = getWiFiCredentialsAmountFromMemory();
  if (amount == 0)
    return 0;

  int removedAmount = 0;

  char *key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  char *buffer = (char *) malloc(STRING_LENGTH * sizeof(char));

  for (int i = 0; i < amount;) {
    sprintf(key, "%s%d", phone_recipient_key_prefix, i);
    memory.getString(key, buffer, STRING_LENGTH);

    if (!strcmp(phone, buffer) && removePhoneRecipientFromMemory(i)) {
      removedAmount++;
      amount--;
    } else {
      i++;
    }
  }

  log_d("%d records of '%s' were removed from memory.", removedAmount, phone);
  
  free(buffer);
  free(key);

  return removedAmount;
}

int getPhoneRecipientsAmountFromMemory() {
  return memory.getUInt(phone_recipients_amount_key, 0);
}

char *getPhoneRecipientFromMemory(int index, char *buffer) {
  char* phone_key = (char*) malloc(MEMORY_KEY_MAX_LENGTH * sizeof(char));
  sprintf(phone_key, "%s%d", phone_recipient_key_prefix, index);
  memory.getString(phone_key, buffer, STRING_LENGTH);
  free(phone_key);
  return buffer;
}

#pragma endregion  Phone recipients