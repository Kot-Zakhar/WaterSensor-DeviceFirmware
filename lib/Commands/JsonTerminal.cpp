#include "JsonTerminal.h"
#include "CommandHandlers.h"
#include "Commands.h"
#include "ResponseMessages.h"
#include <NotificationService.h>
#include <custom_types.h>

error_t defaultOkEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t defaultErrorEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t pingJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t wifiCredsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t emailSmtpJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t emailImapJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t emailRecipientsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t gsmPinJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t gsmRecipientsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t gprsUseNetworkPermissionJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t gprsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t notificationTestEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t sensorValueJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
error_t sensorBoundariesJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

error_t modeJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);
// error_t *restartJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

// error_t *helpJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);

// error_t *notRecognizedJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc);


error_t (* jsonEndpoints[COMMAND_AMOUNT + 1])(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) = {
    pingJsonEndpoint,
    wifiCredsJsonEndpoint,

    emailSmtpJsonEndpoint,
    emailImapJsonEndpoint,
    emailRecipientsJsonEndpoint,

    gsmPinJsonEndpoint,
    gsmRecipientsJsonEndpoint,

    gprsUseNetworkPermissionJsonEndpoint,
    gprsJsonEndpoint,

    notificationTestEndpoint,

    sensorValueJsonEndpoint,
    sensorBoundariesJsonEndpoint,

    modeJsonEndpoint,
    defaultOkEndpoint,
    defaultOkEndpoint,
    defaultErrorEndpoint,
};



/* template for handler

    error_t success = 0;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        break;
    }
    case POST:
    {
        break;
    }
    case DELETE:
    {
        break;
    }
    default:
    {
        success = 2;
        break;
    }
    }

    return success;

*/


/**
 * Json API
 * Every request jsonMessage is an object.
 * ```
 * interface JsonRequestMessageObject {
 *    string command;
 *    'get'|'post'|'delete' method?; 
 *    JsonObject payload?;
 * }
 * ```
 *
 * Every response JsonMessage in an object
 * ```
 * interface JsonResponseMessageObject {
 *    string status; // "OK" | "ERROR"
 *    JsonObject? payload;
 * }
*/
void processJsonMessage(JsonVariant &reqObj, DynamicJsonDocument &resDoc);

char *processJsonMessage(const char *req, size_t reqLen, char *outputBuffer, size_t outputLen, error_t &parsingError) {
    DynamicJsonDocument reqDoc(reqLen + 100);
    DeserializationError err = deserializeJson(reqDoc, req);
    parsingError = err.code();
    if (!err) {
        log_d("Json was deserialized");
        JsonVariant reqObj = reqDoc.as<JsonVariant>();
        DynamicJsonDocument resDoc(JSON_DEFAULT_BUFFER_LENGTH);

        processJsonMessage(reqObj, resDoc);

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

        log_d("Actual command is: %s %d", command.c_str(), command.length());
        for (commandIndex = 0; commandIndex != int(COMMAND_AMOUNT); commandIndex++) {
            log_d("comparing to command %s", commands[commandIndex]);
            if (!command.compareTo(commands[commandIndex]))
                break;
        }
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

error_t defaultErrorEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    resDoc["payload"] = "Command doesn't exist.";
    return 1;
}


error_t pingJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    resDoc["payload"] = "pong";
    return 0;
}


error_t wifiCredsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        int amount = 0;
        struct WiFiCred* credsBuffer = (struct WiFiCred*)malloc(sizeof(struct WiFiCred));
        credsBuffer = wifiCredsGetAll(credsBuffer, amount);

        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + amount * sizeof(struct WiFiCred));
        
        JsonArray wifiRecordArr = doc.createNestedArray("payload");

        log_d("Amount of records: %d", amount);
        for (int i = 0; i < amount; i++) {
            JsonObject record = wifiRecordArr.createNestedObject();

            record["ssid"] = credsBuffer[i].ssid;
            record["password"] = credsBuffer[i].password;
            log_d("%d %s %s", i, credsBuffer[i].ssid, credsBuffer[i].password);
        }

        free(credsBuffer);

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        // TODO: validate payload
        if (reqPayload.is<JsonArray>()) {
            log_d("saving a list of wifi creds..");
            JsonArray records = reqPayload.as<JsonArray>();
            for (int i = 0; i < records.size(); i++) {
                const char *ssid = records[i]["ssid"];
                const char *pwd = records[i]["password"];
                wifiCredsAdd(ssid, pwd);
            }
        } else if (reqPayload.is<JsonObject>()) {
            log_d("saving a record of wifi creds");
            JsonObject record = reqPayload.as<JsonObject>();
            const char *ssid = record["ssid"];
            const char *pwd = record["password"];
            wifiCredsAdd(ssid, pwd);
        } else {
            success = 1;
        }
        break;
    }
    case DELETE:
    {
        if (reqPayload.is<JsonArray>()) {
            JsonArray records = reqPayload.as<JsonArray>();
            for (int i = 0; i < records.size(); i++) {
                wifiCredsDelete(records[i]["ssid"], records[i]["password"]);
            }
        } else if (reqPayload.is<JsonObject>()) {
            JsonObject record = reqPayload.as<JsonObject>();
            wifiCredsDelete(record["ssid"], record["password"]);
        } else if (reqPayload.is<char *>()) {
            const char * ssid = reqPayload.as<const char *>();
            wifiCredsDelete(ssid);
        } else if (reqPayload.is<int>()) {
            int index = reqPayload.as<int>();
            wifiCredsDelete(index);
        } else if (reqPayload.isNull()) {
            wifiCredsDeleteAll();
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

error_t emailSmtpJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;
    struct EmailServerSettings settings;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        emailServerSettingsGet(SMTP_EMAIL_SERVER_TYPE, settings);

        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + 8 * STRING_LENGTH);
        JsonObject payload = doc.createNestedObject("payload");
        payload["server"] = settings.server;
        payload["port"] = settings.port;
        payload["login"] = settings.login;
        payload["password"] = settings.password;
        payload["ssl"] = settings.ssl;

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        // TODO: validate payload
        strlcpy(settings.server, reqPayload["server"], STRING_LENGTH);
        strlcpy(settings.port, reqPayload["port"], STRING_LENGTH);
        strlcpy(settings.login, reqPayload["login"], STRING_LENGTH);
        strlcpy(settings.password, reqPayload["password"], STRING_LENGTH);
        settings.ssl = reqPayload["ssl"];

        emailServerSettingsSet(SMTP_EMAIL_SERVER_TYPE, settings);
        break;
    }
    case DELETE:
    {
        emailServerSettingsDelete(SMTP_EMAIL_SERVER_TYPE);
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

error_t emailImapJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;
    struct EmailServerSettings settings;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        emailServerSettingsGet(IMAP_EMAIL_SERVER_TYPE, settings);

        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + 8 * STRING_LENGTH);
        JsonObject payload = doc.createNestedObject("payload");
        payload["server"] = settings.server;
        payload["port"] = settings.port;
        payload["login"] = settings.login;
        payload["password"] = settings.password;
        payload["ssl"] = settings.ssl;

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        // TODO: validate payload
        strlcpy(settings.server, reqPayload["server"], STRING_LENGTH);
        strlcpy(settings.port, reqPayload["port"], STRING_LENGTH);
        strlcpy(settings.login, reqPayload["login"], STRING_LENGTH);
        strlcpy(settings.password, reqPayload["password"], STRING_LENGTH);
        settings.ssl = reqPayload["ssl"];

        emailServerSettingsSet(IMAP_EMAIL_SERVER_TYPE, settings);
        break;
    }
    case DELETE:
    {
        emailServerSettingsDelete(IMAP_EMAIL_SERVER_TYPE);
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

error_t emailRecipientsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        int amount = emailRecipientsGetAmount();

        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + amount * STRING_LENGTH);
        char *buffer = (char *)malloc(STRING_LENGTH);
        
        JsonArray recipientsArray = doc.createNestedArray("payload");

        log_d("Amount of email recipients: %d", amount);
        for (int i = 0; i < amount; i++) {
            emailRecipientGetByIndex(i, buffer, STRING_LENGTH);
            recipientsArray.add(buffer);
            log_d("%d %s", i, buffer);
        }

        free(buffer);

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        // TODO: validate payload
        if (reqPayload.is<JsonArray>()) {
            log_d("saving a list of email recipients");
            JsonArray records = reqPayload.as<JsonArray>();
            for (int i = 0; i < records.size(); i++) {
                const char *email = records[i];
                log_d("%d: %s", i, email);
                emailRecipientAdd(email);
            }
        } else if (reqPayload.is<const char *>()) {
            log_d("saving recipient");
            const char *record = reqPayload.as<const char *>();
            emailRecipientAdd(record);
        } else {
            success = 1;
        }
        break;
    }
    case DELETE:
    {
        if (reqPayload.is<JsonArray>()) {
            JsonArray records = reqPayload.as<JsonArray>();
            for (int i = 0; i < records.size(); i++) {
                emailRecipientDelete((const char *)records[i]);
            }
        } else if (reqPayload.is<const char *>()) {
            emailRecipientDelete(reqPayload.as<const char *>());
        } else if (reqPayload.is<int>()) {
            emailRecipientDelete(reqPayload.as<int>());
        } else if (reqPayload.isNull()) {
            emailRecipientsDeleteAll();
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


error_t gsmPinJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    
    error_t success = 0;

    switch (method) {
    case POST:
    {
        if (reqPayload.is<String>()) {
            gsmPinSet(reqPayload.as<const char *>());
        } else {
            success = 1;
        }
        break;
    }
    case DELETE:
    {
        gsmPinDelete();
        break;
    }
    case NO_METHOD:
    case GET:
    default:
    {
        success = 2;
        break;
    }
    }

    return success;
}

error_t gsmRecipientsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        int amount = gsmRecipientsGetAmount();

        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + amount * STRING_LENGTH);
        char *buffer = (char *)malloc(STRING_LENGTH);
        
        JsonArray recipientsArray = doc.createNestedArray("payload");

        log_d("Amount of gsm recipients: %d", amount);
        for (int i = 0; i < amount; i++) {
            gsmRecipientGetByIndex(i, buffer, STRING_LENGTH);
            recipientsArray.add(buffer);
            log_d("%d %s", i, buffer);
        }

        free(buffer);

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        // TODO: validate payload
        if (reqPayload.is<JsonArray>()) {
            log_d("saving a list of gsm recipients");
            JsonArray records = reqPayload.as<JsonArray>();
            for (int i = 0; i < records.size(); i++) {
                const char *phoneNumber = records[i];
                log_d("%d: %s", i, phoneNumber);
                gsmRecipientAdd(phoneNumber);
            }
        } else if (reqPayload.is<const char *>()) {
            log_d("saving recipient");
            const char *record = reqPayload.as<const char *>();
            gsmRecipientAdd(record);
        } else {
            success = 1;
        }
        break;
    }
    case DELETE:
    {
        if (reqPayload.is<JsonArray>()) {
            JsonArray records = reqPayload.as<JsonArray>();
            for (int i = 0; i < records.size(); i++) {
                gsmRecipientDelete((const char *)records[i]);
            }
        } else if (reqPayload.is<const char *>()) {
            gsmRecipientDelete(reqPayload.as<const char *>());
        } else if (reqPayload.is<int>()) {
            gsmRecipientDelete(reqPayload.as<int>());
        } else if (reqPayload.isNull()) {
            gsmRecipientsDeleteAll();
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


error_t gprsUseNetworkPermissionJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + 20);
        doc["payload"] = gprsPermGet();

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        if (reqPayload.is<bool>()) {
            gprsPermSet(reqPayload.as<bool>());
        } else {
            success = 1;
        }
        break;
    }
    case DELETE:
    default:
    {
        success = 2;
        break;
    }
    }

    return success;

}

error_t gprsJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;
    GprsSettings settings;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        gprsSettingsGet(settings);

        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + 2 * 3 * STRING_LENGTH);
        JsonObject payload = doc.createNestedObject("payload");
        payload["apn"] = settings.apn;
        payload["password"] = settings.password;
        payload["user"] = settings.user;

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        strlcpy(settings.apn, reqPayload["apn"], STRING_LENGTH);
        strlcpy(settings.password, reqPayload["password"], STRING_LENGTH);
        strlcpy(settings.user, reqPayload["user"], STRING_LENGTH);

        gprsSettingsSet(settings);
        break;
    }
    case DELETE:
    {
        gprsSettingsDelete();
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

error_t notificationTestEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;

    if (reqPayload.is<String>()) {
        String type = reqPayload.as<String>();
        type.toLowerCase();
        if (type.equals("sms")) {
            sendTestSms();
        } else if (type.equals("email-gprs")) {
            sendTestEmailGprs();
        } else if (type.equals("email-wifi")) {
            sendTestEmailWiFi();
        } else {
            success = 1;
        }
    } else {
        success = 1;
    }

    return success;

}


error_t sensorValueJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + 2 * 3 * 10);
        JsonObject payload = doc.createNestedObject("payload");

        SensorsValues values;
        getSensorsValues(values);

        payload["water"] = values.water;
        payload["temperature"] = values.temperature;
        payload["humidity"] = values.humidity;

        resDoc = std::move(doc);
        break;
    }
    case POST:
    case DELETE:
    default:
    {
        success = 2;
        break;
    }
    }

    return success;
}

error_t sensorBoundariesJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    error_t success = 0;

    switch (method) {
    case NO_METHOD:
    case GET:
    {
        DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH + 2 * 6 * 10);
        JsonObject payload = doc.createNestedObject("payload");

        int waterLow, waterHigh;
        float tLow, tHigh, hLow, hHigh;
        bool water = getWaterSensorBoundaries(waterLow, waterHigh);
        bool temp = getTemperatureBoundaries(tLow, tHigh);
        bool humid = getHumidityBoundaries(hLow, hHigh);

        if (water) {
            JsonObject water = payload.createNestedObject("water");
            water["low"] = waterLow;
            water["high"] = waterHigh;
        }

        if (temp) {
            JsonObject temp = payload.createNestedObject("temperature");
            temp["low"] = tLow;
            temp["high"] = tHigh;
        }

        if (humid) {
            JsonObject humid = payload.createNestedObject("humidity");
            humid["low"] = hLow;
            humid["high"] = hHigh;
        }

        resDoc = std::move(doc);
        break;
    }
    case POST:
    {
        JsonObject water = reqPayload.getMember("water");
        if (!water.isNull()) {
            setWaterSensorBoundaries(water["low"], water["high"]);
        }
        JsonObject temp = reqPayload.getMember("temperature");
        if (!temp.isNull()) {
            setTemperatureBoundaries(temp["low"], temp["high"]);
        }
        JsonObject humid = reqPayload.getMember("humidity");
        if (!humid.isNull()) {
            setHumidityBoundaries(humid["low"], humid["high"]);
        }
        break;
    }
    case DELETE:
    {
        if (reqPayload.isNull()) {
            deleteWaterSensorBoundaries();
            deleteTemperatureBoundaries();
            deleteHumidityBoundaries();
        } else if (reqPayload.is<String>()) {
            String sensorType = reqPayload.as<String>();
            if (sensorType.equals("temp"))
                deleteTemperatureBoundaries();
            else if (sensorType.equals("humid"))
                deleteHumidityBoundaries();
            else if (sensorType.equals("water"))
                deleteWaterSensorBoundaries();
        } else if (reqPayload.is<JsonArray>()) {
            JsonArray sensors = reqPayload.as<JsonArray>();
            for (int i = 0; i < sensors.size(); i++) {
                String sensorType = sensors[i];
                if (sensorType.equals("temp"))
                    deleteTemperatureBoundaries();
                else if (sensorType.equals("humid"))
                    deleteHumidityBoundaries();
                else if (sensorType.equals("water"))
                    deleteWaterSensorBoundaries();
            }
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

error_t modeJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
    return 0;
}

// error_t restartJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
//     return createDefaultOKResponse();
// }

// error_t helpJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
//     return createDefaultOKResponse();
// }

// error_t notRecognizedJsonEndpoint(command_method_t method, const JsonVariant &reqPayload, DynamicJsonDocument &resDoc) {
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
  int sensorValue = getWaterSensorValue();
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