#include "BluetoothSerial.h"
#include <Preferences.h>

#include <BtHelper.h>
#include "preferences.cpp"

#define STRING_LENGTH 100

BluetoothSerial SerialBT;
Preferences preferences;

// bt credentials
const char* bt_credentials_name = "ESP32";
// const char* bt_credentials_pin = "esp32password";

// memory keys
const char* wifi_ssid_key_prefix = "ssid";
const char* wifi_password_key_prefix = "pass";
const char* wifi_amount_key = "amount";

// bt messages
const char* wifi_ssid_request_message = "Provide ssid of network:";
const char* wifi_password_request_message = "Provide passowrd of network:";
const char* erasing_memory_message = "Erasing all memory";
const char* status_ok_message = "Ok";
const char* status_error_message = "Error";
const char* unknown_command_message = "Unknown command";
const char* restart_message = "Restarting...";

// char* charBuffer;

unsigned int counter = 0;

void ProcessBt();
void AddCredentialsCommand();
int SaveCredentials(const char* ssid, const char* password);
void PrintNetworksFromMemory(Preferences *memory, BluetoothSerial *SerialBT);
void ClearMemoryCommand();
void RestartESP();

void setup() {
  Serial.begin(115200);
  SerialBT.begin(bt_credentials_name);
  Serial.println("You can pair to ESP32s");

  preferences.begin("bt_wifi", false);
  // preferences.clear();

  PrintNetworksFromMemory(&preferences, NULL);
}

void loop() {
  ProcessBt();
  delay(20);
}


void ProcessBt(){
  if (!SerialBT.available())
    return;

  String commandLine = String(STRING_LENGTH);

  // Serial.println("BT available: starting reading...");

  int command = ReadBtCommand(&SerialBT, &commandLine, STRING_LENGTH);

  Serial.println("Got command: " + String(command));

  switch (command)
  {
  case WIFI_SSID_AND_PASSWORD:
    AddCredentialsCommand();
    break;
  case WIFI_ERASE_MEMORY:
    ClearMemoryCommand();
    break;
  case WIFI_SHOW_NETWORKS:
    PrintNetworksFromMemory(&preferences, &SerialBT);
    break;
  case RESTART:
    RestartESP();
    break;
  default:
    WriteBtLine(&SerialBT, unknown_command_message);
    break;
  }
  
}

void AddCredentialsCommand(){
  String ssid = String(STRING_LENGTH);
  String password = String(STRING_LENGTH);
    
  WriteBtLine(&SerialBT, wifi_ssid_request_message);
  while (!SerialBT.available()){}
  Serial.println("received ssid");
  if (ReadBtLine(&SerialBT, &ssid, STRING_LENGTH) < 0){
    WriteBtLine(&SerialBT, status_error_message);
    return;
  }
  WriteBtLine(&SerialBT, wifi_password_request_message);
  while(!SerialBT.available()){}
  Serial.println("received password");
  if (ReadBtLine(&SerialBT, &password, STRING_LENGTH) < 0){
    WriteBtLine(&SerialBT, status_error_message);
    return;
  }
  SaveCredentials(ssid.c_str(), password.c_str());
  WriteBtLine(&SerialBT, status_ok_message);
}

int SaveCredentials(const char* ssid, const char* password){
  char* ssid_key = (char*) malloc(6 * sizeof(char));
  char* password_key = (char*) malloc(6 * sizeof(char));

  int counter = preferences.getUInt(wifi_amount_key, 0);
  
  sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, counter);
  sprintf(password_key, "%s%d", wifi_password_key_prefix, counter);

  preferences.putString(ssid_key, ssid);
  preferences.putString(password_key, password);

  preferences.putUInt(wifi_amount_key, ++counter);

  Serial.println("saved credantials");
  Serial.println(ssid);
  Serial.println(password);

  free(ssid_key);
  free(password_key);

  return counter;
}

void PrintNetworksFromMemory(Preferences *memory, BluetoothSerial *SerialBT){

  char* ssid_key = (char*) malloc(6 * sizeof(char));
  char* password_key = (char*) malloc(6 * sizeof(char));

  char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
  char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

  int counter = memory->getUInt(wifi_amount_key, 0);

  if (counter == 0){
    if (SerialBT != NULL)
      WriteBtLine(SerialBT, "No networks in memory");
    else
      Serial.println("No networks in memory");
  } else {
    for (int i = 0; i < counter; i++){
      sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, i);
      sprintf(password_key, "%s%d", wifi_password_key_prefix, i);

      memory->getString(ssid_key, ssid, STRING_LENGTH);
      memory->getString(password_key, password, STRING_LENGTH);
      
      String output = String(i) + ": '" + String(ssid) + "' - '" + String(password) + "'";

      if (SerialBT != NULL)
        WriteBtLine(SerialBT, output.c_str());
      else
        Serial.println(output);
    }
  }

  free(ssid_key);
  free(password_key);
  free(ssid);
  free(password);
}

void ClearMemoryCommand(){
  preferences.clear();
  WriteBtLine(&SerialBT, erasing_memory_message);
}

void RestartESP(){
  WriteBtLine(&SerialBT, restart_message);
  ESP.restart();
}