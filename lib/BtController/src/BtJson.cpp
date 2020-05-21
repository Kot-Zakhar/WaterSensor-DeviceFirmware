#include <BtJson.h>

void SendPongJson();
void AddWiFiCredentialsJson(JsonObject wifiRecord);
void RemoveWifiSingleJson(JsonObject payload);
void SendNetworksFromMemoryJson();
void GetEmailSettingsJson();
void SetEmailSettingsJson(JsonObject payload);
void GetModeJson();
void SwitchModeJson(JsonObject payload);
void RestartESPJson();

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
  int commandIndex;
  for (commandIndex = 0; commandIndex < COMMAND_AMOUNT; commandIndex++){
    if (!strcmp(command, commands[commandIndex]))
      break;
  }
  // result is in commandIndex

  if (commandIndex == COMMAND_AMOUNT){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Cmd not recognized.");
    IOIndicate(BT_GOT_BAD_COMMAND);
  } else {
    IOWrite(IO_WRITE_SCREEN, ("JSON " + String(command)).c_str());
    IOIndicate(BT_GOT_GOOD_COMMAND);
    JsonObject payload = reqMessage["payload"];

    switch (commandIndex)
    {
    case BT_PING:
      SendPongJson();
      break;
    case WIFI_SSID_AND_PASSWORD:
      AddWiFiCredentialsJson(payload);
      break;
  //   case JSON_WIFI_REMOVE_ALL:
  //     RemoveAllWiFiCredentialsJsonCommand();
  //     break;
    case WIFI_REMOVE_SINGLE:
      RemoveWifiSingleJson(payload);
      break;
    case WIFI_SHOW_NETWORKS:
      SendNetworksFromMemoryJson();
      break;
    case SMTP_SETTINGS:
      GetEmailSettingsJson();
      break;
    case SET_SMTP_SETTINGS:
      SetEmailSettingsJson(payload);
      break;
    case RESTART:
      RestartESPJson();
      break;
    case GET_MODE:
      GetModeJson();
      break;
    case SWITCH_MODE:
      SwitchModeJson(payload);
      break;
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
  // res["payload"] = "pong";
  SendJsonResponse(doc);
}

void SendNetworksFromMemoryJson() {

  int counter = GetWiFiCredentialsAmountFromMemory();

  DynamicJsonDocument doc(20 + 60 * counter);
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

void AddWiFiCredentialsJson(JsonObject wifiRecord) {
  const char *ssid = wifiRecord["ssid"];
  const char *pwd = wifiRecord["password"];

  SaveWiFiCredentialsInMemory(ssid, pwd);

  DynamicJsonDocument answer(20);
  answer["status"] = "OK";

  SendJsonResponse(answer);
}

void RemoveWifiSingleJson(JsonObject payload) {
  int index = payload["index"];
  // TODO: validate index
  bool removed = RemoveWiFiCredentials(index);

  DynamicJsonDocument answer(20);
  answer["status"] = removed ? "OK" : "Invalid index specified";
  SendJsonResponse(answer);
}


void GetEmailSettingsJson() {
  DynamicJsonDocument answer(STRING_LENGTH * EMAIL_SETTINGS_COUNT);
  answer["status"] = "OK";
  JsonObject payload = answer.createNestedObject("payload");
  char *buffer = (char *)malloc(STRING_LENGTH);
  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++) {
    // TODO: check if values are initialized
    // On first start they are nulls
    GetEmailValue(i, buffer);
    payload[email_settings[i]] = buffer;
  }

  free(buffer);
  SendJsonResponse(answer);
}

void SetEmailSettingsJson(JsonObject payload) {
  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";

  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++) {
    const char *value = payload[email_settings[i]];
    // TODO: Check if this if really do its job
    if (value != NULL)
      SetEmailValue(i, value);
  }

  SendJsonResponse(response);
}

void SwitchModeJson(JsonObject payload) {
  bool config = payload["config"];
  SetStateInMemory(config);

  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";
  SendJsonResponse(response);
}

void GetModeJson() {
  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";
  bool config = IsConfigStateInMemory();
  response.createNestedObject("payload")["config"] = config;
  SendJsonResponse(response);
}

void RestartESPJson() {
  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";
  SendJsonResponse(response);

  delay(1000);
  ESP.restart();
}

void SendNotRecognizedJson() {
  DynamicJsonDocument doc(50);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "Not recognized";
  SendJsonResponse(doc);
}

void SendJsonResponse(JsonDocument &res) {
  // BluetoothSerial *bt = GetCurrentBtSerial();
  // serializeJson(res, *bt);
  int length = measureJson(res) + 1;

  char *buffer = (char *) malloc(length);
  serializeJson(res, buffer, length);
  WriteBt(buffer);
  free(buffer);
}