#include "BtTerminalController.h"

BluetoothSerial *BT;
Preferences *memory;


// bt messages
const char* wifi_ssid_request_message = "Provide ssid of network:";
const char* wifi_password_request_message = "Provide passowrd of network:";
const char* erasing_memory_message = "Erasing all memory";
const char* status_ok_message = "Ok";
const char* status_error_message = "Error";
const char* unknown_command_message = "Unknown command";
const char* restart_message = "Restarting...";

// memory keys
const char* wifi_table_name = "bt_wifi";
const char* wifi_ssid_key_prefix = "ssid";
const char* wifi_password_key_prefix = "pass";
const char* wifi_amount_key = "amount";

void ProcessBt();
void AddCredentialsCommand();
int SaveCredentials(const char* ssid, const char* password);
void PrintNetworksFromMemory(Preferences *memory, BluetoothSerial *BT);
void ClearMemoryCommand();
void RestartESPCommand();

void BtControllerSetup(BluetoothSerial *SerialBT, Preferences *preferences){
    BT = SerialBT;
    memory = preferences;
}

void ProcessBt(){
  if (!BT->available())
    return;

    Serial.println("here");

  String commandLine = String(STRING_LENGTH);

  // Serial.println("BT available: starting reading...");

  int command = ReadBtCommand(BT, &commandLine, STRING_LENGTH);

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
    PrintNetworksFromMemory(memory, BT);
    break;
  case RESTART:
    RestartESPCommand();
    break;
  default:
    WriteBtLine(BT, unknown_command_message);
    break;
  }
  
}

void AddCredentialsCommand(){
  String ssid = String(STRING_LENGTH);
  String password = String(STRING_LENGTH);
    
  WriteBtLine(BT, wifi_ssid_request_message);
  while (!BT->available()){}
  Serial.println("received ssid");
  if (ReadBtLine(BT, &ssid, STRING_LENGTH) < 0){
    WriteBtLine(BT, status_error_message);
    return;
  }
  WriteBtLine(BT, wifi_password_request_message);
  while(!BT->available()){}
  Serial.println("received password");
  if (ReadBtLine(BT, &password, STRING_LENGTH) < 0){
    WriteBtLine(BT, status_error_message);
    return;
  }
  SaveCredentials(ssid.c_str(), password.c_str());
  WriteBtLine(BT, status_ok_message);
}

int SaveCredentials(const char* ssid, const char* password){
  char* ssid_key = (char*) malloc(6 * sizeof(char));
  char* password_key = (char*) malloc(6 * sizeof(char));

  int counter = memory->getUInt(wifi_amount_key, 0);
  
  sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, counter);
  sprintf(password_key, "%s%d", wifi_password_key_prefix, counter);

  memory->putString(ssid_key, ssid);
  memory->putString(password_key, password);

  memory->putUInt(wifi_amount_key, ++counter);

  Serial.println("saved credantials");
  Serial.println(ssid);
  Serial.println(password);

  free(ssid_key);
  free(password_key);

  return counter;
}

void PrintNetworksFromMemory(Preferences *memory, BluetoothSerial *BT){

  char* ssid_key = (char*) malloc(6 * sizeof(char));
  char* password_key = (char*) malloc(6 * sizeof(char));

  char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
  char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

  int counter = memory->getUInt(wifi_amount_key, 0);

  if (counter == 0){
    if (BT != NULL)
      WriteBtLine(BT, "No networks in memory");
    else
      Serial.println("No networks in memory");
  } else {
    for (int i = 0; i < counter; i++){
      sprintf(ssid_key, "%s%d", wifi_ssid_key_prefix, i);
      sprintf(password_key, "%s%d", wifi_password_key_prefix, i);

      memory->getString(ssid_key, ssid, STRING_LENGTH);
      memory->getString(password_key, password, STRING_LENGTH);
      
      String output = String(i) + ": '" + String(ssid) + "' - '" + String(password) + "'";

      if (BT != NULL)
        WriteBtLine(BT, output.c_str());
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
  memory->clear();
  WriteBtLine(BT, erasing_memory_message);
}

void RestartESPCommand(){
  WriteBtLine(BT, restart_message);
  ESP.restart();
}