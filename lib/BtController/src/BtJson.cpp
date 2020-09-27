#include <BtJson.h>

void sendPongJson();
void addWiFiCredentialsJson(JsonObject wifiRecord);
void removeWifiSingleJson(JsonObject payload);
void sendNetworksFromMemoryJson();
void getEmailSettingsJson();
void setEmailSettingsJson(JsonObject payload);
void getModeJson();
void switchModeJson(JsonObject payload);
void getSensorValueJson();
void restartESPJson();

void sendNotRecognizedJson();


void sendJsonResponse(JsonDocument &res);

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

void processBtJsonMessage(JsonObject &reqMessage) {

  // parsing the command
  const char* command = reqMessage["command"];
  int commandIndex;
  for (commandIndex = 0; commandIndex < COMMAND_AMOUNT; commandIndex++){
    // log_d("Comparing two strings: actual command '%s' vs template '%s'", command, commands[commandIndex]);
    if (!strcmp(command, commands[commandIndex]))
      break;
  }
  // result is in commandIndex

  if (commandIndex == COMMAND_AMOUNT){
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Cmd not recognized.");
    ioIndicate(BT_GOT_BAD_COMMAND);
  } else {
    // IOWrite(IO_WRITE_SCREEN, ("JSON " + String(command)).c_str());
    ioIndicate(BT_GOT_GOOD_COMMAND);
    JsonObject payload = reqMessage["payload"];

    switch (commandIndex)
    {
    case BT_PING:
      sendPongJson();
      break;
    case WIFI_SSID_AND_PASSWORD:
      addWiFiCredentialsJson(payload);
      break;
  //   case JSON_WIFI_REMOVE_ALL:
  //     RemoveAllWiFiCredentialsJsonCommand();
  //     break;
    case WIFI_REMOVE_SINGLE:
      removeWifiSingleJson(payload);
      break;
    case WIFI_SHOW_NETWORKS:
      sendNetworksFromMemoryJson();
      break;
    case SMTP_SETTINGS:
      getEmailSettingsJson();
      break;
    case SET_SMTP_SETTINGS:
      setEmailSettingsJson(payload);
      break;
    case RESTART:
      restartESPJson();
      break;
    case GET_MODE:
      getModeJson();
      break;
    case SENSOR_VALUE:
      getSensorValueJson();
      break;
    case SWITCH_MODE:
      switchModeJson(payload);
      break;
    default:
      sendNotRecognizedJson();
      break;
    }
  }
}

void sendPongJson() {
  DynamicJsonDocument doc(50);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "OK";
  // res["payload"] = "pong";
  sendJsonResponse(doc);
}

void sendNetworksFromMemoryJson() {

  int counter = getWiFiCredentialsAmountFromMemory();

  DynamicJsonDocument doc(20 + 60 * counter);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "OK";
  JsonArray wifiRecordArr = res.createNestedArray("payload");

  if (counter == 0){
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

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

  sendJsonResponse(doc);
}

void addWiFiCredentialsJson(JsonObject wifiRecord) {
  const char *ssid = wifiRecord["ssid"];
  const char *pwd = wifiRecord["password"];

  SaveWiFiCredentialsInMemory(ssid, pwd);

  DynamicJsonDocument answer(20);
  answer["status"] = "OK";

  sendJsonResponse(answer);
}

void removeWifiSingleJson(JsonObject payload) {
  int index = payload["index"];
  // TODO: validate index
  bool removed = RemoveWiFiCredentials(index);

  DynamicJsonDocument answer(20);
  answer["status"] = removed ? "OK" : "Invalid index specified";
  sendJsonResponse(answer);
}


void getEmailSettingsJson() {
  DynamicJsonDocument answer(STRING_LENGTH * EMAIL_SETTINGS_COUNT);
  answer["status"] = "OK";
  JsonObject payload = answer.createNestedObject("payload");
  char *buffer = (char *)malloc(STRING_LENGTH);
  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++) {
    // TODO: check if values are initialized
    // On first start they are nulls
    getEmailValue(i, buffer);
    payload[email_settings[i]] = buffer;
  }

  free(buffer);
  sendJsonResponse(answer);
}

void setEmailSettingsJson(JsonObject payload) {
  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";

  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++) {
    const char *value = payload[email_settings[i]];
    // TODO: Check if this if really do its job
    if (value != NULL)
      SetEmailValue(i, value);
  }

  sendJsonResponse(response);
}

void switchModeJson(JsonObject payload) {
  bool config = payload["config"];
  setStateInMemory(config);

  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";
  sendJsonResponse(response);
}

void getModeJson() {
  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";
  bool config = isConfigStateInMemory();
  response.createNestedObject("payload")["config"] = config;
  sendJsonResponse(response);
}

void getSensorValueJson() {
  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";
  int sensorValue = getSensorValue();
  response["payload"] = sensorValue;
  sendJsonResponse(response);
}

void restartESPJson() {
  DynamicJsonDocument response(STRING_LENGTH);
  response["status"] = "OK";
  sendJsonResponse(response);

  delay(1000);
  ESP.restart();
}

void sendNotRecognizedJson() {
  DynamicJsonDocument doc(50);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "Not recognized";
  sendJsonResponse(doc);
}

void sendJsonResponse(JsonDocument &res) {
  // BluetoothSerial *bt = GetCurrentBtSerial();
  // serializeJson(res, *bt);
  int length = measureJson(res) + 1;

  char *buffer = (char *) malloc(length);
  log_d("Sending buffer to client:\n%s", buffer);
  serializeJson(res, buffer, length);
  writeBt(buffer);
  free(buffer);
}