#include "JsonTerminal.h"
#include "CommandHandlers.h"
#include "Commands.h"
#include "ResponseMessages.h"
#include <custom_types.h>

error_t defaultOkEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t pingJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t wifiCredsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
// JsonObject *emailSmtpJsonEndpoint(command_method_t method, JsonVariant &payload);
// JsonObject *emailImapJsonEndpoint(command_method_t method, JsonVariant &payload);
// JsonObject *emailRecipientsJsonEndpoint(command_method_t method, JsonVariant &payload);

// JsonObject *gsmPinJsonEndpoint(command_method_t method, JsonVariant &payload);
// JsonObject *gsmUseNetworkPermissionJsonEndpoint(command_method_t method, JsonVariant &payload);
// JsonObject *gsmRecipientsJsonEndpoint(command_method_t method, JsonVariant &payload);

// JsonObject *sensorValueJsonEndpoint(command_method_t method, JsonVariant &payload);
// JsonObject *sensorBoundariesJsonEndpoint(command_method_t method, JsonVariant &payload);

// JsonObject *modeJsonEndpoint(command_method_t method, JsonVariant &payload);
// JsonObject *restartJsonEndpoint(command_method_t method, JsonVariant &payload);

// JsonObject *helpJsonEndpoint(command_method_t method, JsonVariant &payload);

// JsonObject *notRecognizedJsonEndpoint(command_method_t method, JsonVariant &payload);


error_t (* jsonEndpoints[COMMAND_AMOUNT + 1])(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) = {
    pingJsonEndpoint,
    wifiCredsJsonEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    // emailSmtpJsonEndpoint,
    // emailImapJsonEndpoint,
    // emailRecipientsJsonEndpoint,
    // gsmPinJsonEndpoint,
    // gsmUseNetworkPermissionJsonEndpoint,
    // gsmRecipientsJsonEndpoint,
    // sensorValueJsonEndpoint,
    // sensorBoundariesJsonEndpoint,
    // modeJsonEndpoint,
    // restartJsonEndpoint,
    // helpJsonEndpoint,
    // notRecognizedJsonEndpoint
};


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
void processJsonMessage(JsonVariant &reqObj, DynamicJsonDocument &resVariant);

char *processJsonMessage(const char *req, size_t reqLen, char *outputBuffer, size_t outputLen, error_t &parsingError) {
    DynamicJsonDocument reqDoc(reqLen + 100);
    DeserializationError err = deserializeJson(reqDoc, req);
    parsingError = err.code();
    if (!err) {
        log_d("Json was deserialized");
        JsonVariant reqObj = reqDoc.as<JsonVariant>();
        DynamicJsonDocument resDoc(JSON_DEFAULT_LONG_BUFFER_LENGTH);
        // JsonVariant resObj = resDoc.as<JsonVariant>();
        processJsonMessage(reqObj, resDoc);

        resDoc.shrinkToFit();

        int responseLen = measureJson(resDoc) + 1;

        if (responseLen > outputLen) {
            outputBuffer = (char *)realloc(outputBuffer, responseLen);
            outputLen = responseLen;
        }

        serializeJson(resDoc, outputBuffer, outputLen);
    } else {
        log_d("Failing at deserializing json");
    }
    return outputBuffer;
}

void processJsonMessage(JsonVariant &reqObj, DynamicJsonDocument &resDoc) {
    JsonVariant payload = reqObj.getMember("payload");
    
    int commandIndex;
    JsonVariant commandVariant = reqObj.getMember("command");
    if (commandVariant.isNull()) {
        commandIndex = COMMAND_AMOUNT;
        log_d("command is null");
    } else {
        String command = commandVariant.as<String>();
        command.toLowerCase();
        command.trim();
        for (commandIndex = 0;
            commandIndex != int(COMMAND_AMOUNT) &&
            (strcmp(commands[commandIndex], command.c_str()));
            commandIndex++);
        log_d("Command index: %d %s", commandIndex, commands[commandIndex]);
    }

    int methodIndex;
    JsonVariant methodVariant = reqObj.getMember("method");
    if (methodVariant.isNull()) {
        methodIndex = NO_METHOD;
    } else {
        String method = methodVariant.as<String>();
        method.toLowerCase();
        method.trim();
        for (methodIndex = 0;
            methodIndex != int(METHOD_AMOUNT) &&
            (strcmp(method.c_str(), commandMethods[methodIndex]) != 0);
            methodIndex++);
        if (methodIndex == METHOD_AMOUNT)
            methodIndex = NO_METHOD;
    }
    log_d("method index: %d", methodIndex);
    
    error_t error = (*jsonEndpoints[commandIndex])((command_method_t)methodIndex, payload, resDoc);

    if (error) {
        resDoc["status"] = status_error_message;
    } else {
        resDoc["status"] = status_ok_message;
    }
}

error_t defaultOkEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    return 0;
}

error_t pingJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    resDoc["payload"] = "pong";
    return 0;
}




error_t wifiCredsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;
    // int amount = wifiCredsGetAmount();


    // DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + amount * sizeof(struct WiFiCred));
    // JsonObject res = doc.as<JsonObject>();
    // JsonObject res;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        int amount = 0;
        struct WiFiCred* credsBuffer = (struct WiFiCred*)malloc(sizeof(struct WiFiCred));
        credsBuffer = wifiCredsGetAll(credsBuffer, amount);
        
        JsonArray wifiRecordArr = resDoc.createNestedArray("payload");

        log_d("Amount of records: %d", amount);
        for (int i = 0; i < amount; i++) {
            JsonObject record = wifiRecordArr.createNestedObject();

            record["ssid"] = credsBuffer[i].ssid;
            record["password"] = credsBuffer[i].password;
            log_d("%d %s %s", i, credsBuffer[i].ssid, credsBuffer[i].password);
        }

        free(credsBuffer);
        break;
    }
    case POST:
    {
        if (reqPayload.is<JsonArray>()) {
            JsonArray records = reqPayload.as<JsonArray>();
            for (int i = 0; i < records.size(); i++) {
                const char *ssid = records[i]["ssid"];
                const char *pwd = records[i]["password"];
                wifiCredsAdd(ssid, pwd);
            }
        } else if (reqPayload.is<JsonObject>()) {
            JsonObject record = reqPayload.as<JsonObject>();
            const char *ssid = record["ssid"];
            const char *pwd = record["password"];
            wifiCredsAdd(ssid, pwd);
        } else {
            success = 1;
        }
        break;
    }
    default:
    {
        success = 2;
        break;
    }
    }


    return success;
}

// JsonObject emailSmtpJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject emailImapJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject emailRecipientsJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject gsmPinJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject gsmUseNetworkPermissionJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject gsmRecipientsJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject sensorValueJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject sensorBoundariesJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject modeJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject restartJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject helpJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultOKResponse();
// }

// JsonObject notRecognizedJsonEndpoint(command_method_t method, JsonVariant &payload) {
//     return createDefaultERRORResponse();
// }




/*

void sendPongJson() {
  DynamicJsonDocument doc(50);
  JsonObject res = doc.to<JsonObject>();
  res["status"] = "OK";
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
        getWiFiSsidFromMemory(i, ssid);
        getWiFiPasswordFromMemory(i, password);
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

  saveWiFiCredentialsToMemory(ssid, pwd);

  DynamicJsonDocument answer(20);
  answer["status"] = "OK";

  sendJsonResponse(answer);
}

void removeWifiSingleJson(JsonObject payload) {
  int index = payload["index"];
  // TODO: validate index
  bool removed = removeWiFiCredentialsFromMemory(index);

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
    getEmailValueFromMemory(i, buffer);
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
      setEmailValueToMemory(i, value);
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



*/