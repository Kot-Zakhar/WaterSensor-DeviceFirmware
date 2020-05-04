#include <BtJson.h>

void SendPongJson();
void SendNetworksFromMemoryJson();
void SendNotRecognizedJson();


void SendJsonResponse(JsonDocument &res);

// void AddWiFiCredentialsJsonCommand();

/**
 * Json API
 * Every request jsonMessage is an object.
 * ```
 * interface JsonRequestMessageObject {
 *    string command;
 *    JsonObject? payload;
 * }
 * ```
 * 
 * Every response JsonMessage in an object
 * ```
 * interface JsonResponseMessageObject {
 *    string status; // "OK" | message which describes and error
 *    JsonObject? payload;
 * }
*/

void ProcessBtJsonMessage(JsonObject &reqMessage) {

  // parsing the command
  const char* command = reqMessage["command"];
  String line = String(command);
  int commandIndex;
  for (commandIndex = 0; commandIndex < COMMAND_AMOUNT; commandIndex++){
    if (!line.compareTo(String(commands[commandIndex])))
      break;
  }
  // result is in commandIndex

  if (commandIndex == COMMAND_AMOUNT){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Cmd not recognized.");
  } else {
    JsonObject payload;

    switch (commandIndex)
    {
    case BT_PING:
      SendPongJson();
      break;
    // case JSON_WIFI_SSID_AND_PASSWORD:
    //   AddWiFiCredentialsJsonCommand();
    //   break;
  //   case JSON_WIFI_ERASE:
  //     RemoveAllWiFiCredentialsJsonCommand();
  //     break;
    case WIFI_SHOW_NETWORKS:
      SendNetworksFromMemoryJson();
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
    default:
      SendNotRecognizedJson();
      break;
    }
  }
}

void SendPongJson() {
  DynamicJsonDocument doc(50);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "OK";
  SendJsonResponse(doc);
}

void SendNetworksFromMemoryJson() {

  int counter = GetWiFiCredentialsAmountFromMemory();

  DynamicJsonDocument doc(1024);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "OK";
  JsonArray wifiRecordArr = res.createNestedArray("payload");

  if (counter == 0){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

    for (int i = 0; i < counter; i++){
        GetWiFiSsidFromMemory(i, ssid);
        GetWiFiPasswordFromMemory(i, password);
        JsonObject record = wifiRecordArr.createNestedObject();
        record["ssid"] = ssid;
        record["password"] = password;
    }

    free(ssid);
    free(password);
  }

  SendJsonResponse(doc);
}

// void AddWiFiCredentialsJsonCommand() {
//   DynamicJsonDocument jsonWifiRecord(200);

//   BluetoothSerial *bt = GetCurrentBtSerial();

//   deserializeJson(jsonWifiRecord, *bt);

//   const char *ssid = jsonWifiRecord["ssid"];
//   const char *pwd = jsonWifiRecord["password"];

//   SaveWiFiCredentialsInMemory(ssid, pwd);

//   WriteBt("ok");
// }

void SendNotRecognizedJson() {
  DynamicJsonDocument doc(50);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "Not recognized";
  SendJsonResponse(doc);
}

void SendJsonResponse(JsonDocument &res) {
  BluetoothSerial *bt = GetCurrentBtSerial();
  serializeJson(res, *bt);
}