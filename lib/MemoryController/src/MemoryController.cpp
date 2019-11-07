#include <MemoryController.h>

// memory keys
const char* wifi_table_name = "bt_wifi";
const char* wifi_ssid_key_prefix = "ssid";
const char* wifi_password_key_prefix = "pass";
const char* wifi_amount_key = "amount";

Preferences memory;

void InitMemoryController(){
    memory.begin(wifi_table_name, false);
}

int SaveCredentialsInMemory(const char* ssid, const char* password){
  char* ssid_key = (char*) malloc(6 * sizeof(char));
  char* password_key = (char*) malloc(6 * sizeof(char));

  int counter = GetCredentialsAmountFromMemory();
  
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

void RemoveCredentialsFromMemory(int index){

}

void RemoveCredentialsFromMemory(const char* ssid){

}

int GetCredentialsAmountFromMemory(){
  return memory.getUInt(wifi_amount_key, 0);
}

char* GetSsidFromMemory(int index, char* buffer){
  char* ssid_key = (char*) malloc(6 * sizeof(char));
  sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, index);
  memory.getString(ssid_key, buffer, STRING_LENGTH);
  free(ssid_key);
  return buffer;
}

char* GetPasswordFromMemory(int index, char* buffer){
  char* password_key = (char*) malloc(6 * sizeof(char));
  sprintf(password_key, "%s%d", wifi_password_key_prefix, index);
  memory.getString(password_key, buffer, STRING_LENGTH);
  free(password_key);
  return buffer;
}

void ClearMemory(){
    memory.clear();
}