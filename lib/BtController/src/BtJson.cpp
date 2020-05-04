#include <BtJson.h>

void PrintNetworksFromMemoryJsonCommand();
void AddWiFiCredentialsJsonCommand();

void ProcessBtJsonCommand(int command) {
  if (command < 0){
    IOIndicate(BT_GOT_BAD_COMMAND);
  } else {
    IOIndicate(BT_GOT_GOOD_COMMAND);
  }
  
  switch (command)
  {
//   case JSON_BT_PING:
//     PingJsonCommand();
//     break;
  case JSON_WIFI_SSID_AND_PASSWORD:
    AddWiFiCredentialsJsonCommand();
    break;
//   case JSON_WIFI_ERASE:
//     RemoveAllWiFiCredentialsJsonCommand();
//     break;
  case JSON_WIFI_SHOW_NETWORKS:
    PrintNetworksFromMemoryJsonCommand();
    break;
//   case JSON_SMTP_SETTINGS:
//     SmtpConfigureJsonCommand();
//     break;
//   case JSON_RESTART:
//     RestartESPJsonCommand();
//     break;
//   case JSON_SWITCH_MODE:
//     SwitchModeJsonCommand();
//     break;
//   case JSON_HELP:
//     HelpJsonCommand();
//     break;
//   case JSON_NOT_RECOGNISED:
//     WriteBt(unknown_command_message);
//     break;
  }
}


void PrintNetworksFromMemoryJsonCommand() {

  int counter = GetWiFiCredentialsAmountFromMemory();

  DynamicJsonDocument doc(1024);
  JsonArray rootArr = doc.to<JsonArray>();

  if (counter == 0){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

    for (int i = 0; i < counter; i++){
        GetWiFiSsidFromMemory(i, ssid);
        GetWiFiPasswordFromMemory(i, password);
        JsonObject record = rootArr.createNestedObject();
        record["ssid"] = ssid;
        record["password"] = password;
    }

    free(ssid);
    free(password);
  }

  int length = measureJson(rootArr) + 1;

  char* buffer = (char*) malloc(length);

  serializeJson(rootArr, buffer, length);

  WriteBt(buffer);

  free(buffer);
}

void AddWiFiCredentialsJsonCommand() {
  DynamicJsonDocument jsonWifiRecord(200);

  BluetoothSerial *bt = GetCurrentBtSerial();

  deserializeJson(jsonWifiRecord, *bt);

  const char *ssid = jsonWifiRecord["ssid"];
  const char *pwd = jsonWifiRecord["password"];

  SaveWiFiCredentialsInMemory(ssid, pwd);

  WriteBt("ok");
}