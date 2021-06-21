#include "HttpServerController.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LITTLEFS.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <esp_log.h>

#include <JsonTerminal.h>
#include "CommandHandlers.h"
#include <custom_defaults.h>
#include <ResponseMessages.h>

#include <NotificationService.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void wifiCredsGetDeleteHandlerFunction(AsyncWebServerRequest *req);
void wifiCredsPostPutHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void emailServerSettingsGetDeleteHandlerFunction(AsyncWebServerRequest *req);
void emailSmtpServerSettingsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);
void emailImapServerSettingsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void emailRecipientsGetDeleteHandlerFunction(AsyncWebServerRequest *req);
void emailRecipientsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void gsmPinGetDeleteHandlerFunction(AsyncWebServerRequest *req);
void gsmPinPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void gsmRecipientsGetDeleteHandlerFunction(AsyncWebServerRequest *req);
void gsmRecipientsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void gprsNetworkPermGetHanderFunction(AsyncWebServerRequest *req);
void gprsNetworkPermPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void gprsSettingsGetDeleteHanderFunction(AsyncWebServerRequest *req);
void gprsSettingsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void gsmTestHandlerFunction(AsyncWebServerRequest *req);
void gprsEmailTestHandlerFunction(AsyncWebServerRequest *req);

void sensorsBoundariesGetDeleteHanderFunction(AsyncWebServerRequest *req);
void sensorsBoundariesPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body);

void sensorsValuesGetHandlerFunction(AsyncWebServerRequest *req);


void initHttpServer() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.on("^\\/api\\/wifi-creds(\\/([0-9]*))?$", HTTP_GET | HTTP_DELETE , wifiCredsGetDeleteHandlerFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/wifi-creds", wifiCredsPostPutHandlerFunction));

    server.on("^\\/api\\/email-(smtp|imap)\\/?$", HTTP_GET | HTTP_DELETE, emailServerSettingsGetDeleteHandlerFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/email-smtp", emailSmtpServerSettingsPostHandlerFunction));
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/email-imap", emailImapServerSettingsPostHandlerFunction));

    server.on("^\\/api\\/email-recipients(\\/([0-9]*))?$", HTTP_GET | HTTP_DELETE , emailRecipientsGetDeleteHandlerFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/email-recipients", emailRecipientsPostHandlerFunction));

    server.on("/api/gsm-pin", HTTP_GET | HTTP_DELETE , gsmPinGetDeleteHandlerFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/gsm-pin", gsmPinPostHandlerFunction));

    server.on("^\\/api\\/gsm-recipients(\\/([0-9]*))?$", HTTP_GET | HTTP_DELETE , gsmRecipientsGetDeleteHandlerFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/gsm-recipients", gsmRecipientsPostHandlerFunction));

    server.on("/api/gsm-test", HTTP_GET, gsmTestHandlerFunction);

    server.on("/api/gprs-use-network-perm", HTTP_GET, gprsNetworkPermGetHanderFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/gprs-use-network-perm", gprsNetworkPermPostHandlerFunction));

    server.on("/api/gprs", HTTP_GET | HTTP_DELETE, gprsSettingsGetDeleteHanderFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/gprs", gprsSettingsPostHandlerFunction));

    server.on("/api/gprs-test", HTTP_GET, gprsEmailTestHandlerFunction);

    server.on("/api/sensors-boundaries", HTTP_GET | HTTP_DELETE, sensorsBoundariesGetDeleteHanderFunction);
    server.addHandler(new AsyncCallbackJsonWebHandler("/api/sensors-boundaries", sensorsBoundariesPostHandlerFunction));

    server.on("/api/sensors-values", HTTP_GET, sensorsValuesGetHandlerFunction);

    server
        .serveStatic("/", LITTLEFS, "/public")
        .setDefaultFile("/index.html");

    server.onNotFound([](AsyncWebServerRequest *req) {
        if (req->method() == HTTP_OPTIONS) {
            AsyncWebServerResponse *res = req->beginResponse(200);
            res->addHeader("Access-Control-Allow-Headers", "Content-Type, Accept");
            res->addHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE, PUT");
            req->send(res);
        } else {
            req->send(404);
        }
    });

    server.begin();
}

// void initWebSocket() {
//     ws.onEvent([](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
//         if (type == WS_EVT_CONNECT) {
//             log_d("New ws connection: %s %u", server->url(), client->id());
//             client->ping();
//         } else if (type == WS_EVT_DISCONNECT) {
//             log_d("Client has been disconnected: %s %u", server->url(), client->id());
//         } else if (type == WS_EVT_DATA) {
//             SensorsValues values;
//             getSensorsValues(values);
//             DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
//             doc["water"] = values.water;
//             doc["temp"] = values.temperature;
//             doc["humid"] = values.humidity;
//             size_t len = measureJson(doc);

//             AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(len); //  creates a buffer (len + 1) for you.
//             if (buffer) {
//                 serializeJson(doc, buffer);
//                 client->text(buffer);
//             }
//         }
//     });
// }

void wifiCredsGetDeleteHandlerFunction (AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET:
        wifiCredsJsonEndpoint(GET, reqDoc.to<JsonVariant>(), doc);
        doc["status"] = status_ok_message;
        break;
    case HTTP_DELETE:
        if (!req->pathArg(1).isEmpty()) {
            log_d("removing %s-th record", req->pathArg(1));
            wifiCredsDelete(req->pathArg(1).toInt());
        } else {
            wifiCredsDeleteAll();
        }
        doc["status"] = status_ok_message;
        break;
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void wifiCredsPostPutHandlerFunction (AsyncWebServerRequest *req, JsonVariant &body) {
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    error_t err = wifiCredsJsonEndpoint(POST, body, doc);
    if (err) {
        doc["status"] = status_error_message;
        res->setCode(502);
    } else {
        doc["status"] = status_ok_message;
        res->setCode(200);
    }

    serializeJson(doc, *res);

    req->send(res);
}

void emailServerSettingsGetDeleteHandlerFunction(AsyncWebServerRequest *req) {
    email_server_type_t serverType = req->pathArg(0).equals("smtp") ? SMTP_EMAIL_SERVER_TYPE : IMAP_EMAIL_SERVER_TYPE;
    struct EmailServerSettings settings;

    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET: {
        error_t err = emailServerSettingsGet(serverType, settings);

        if (err) {
            doc["status"] = status_error_message;
        } else {
            DynamicJsonDocument newDoc(JSON_DEFAULT_BUFFER_LENGTH + 8 * STRING_LENGTH);
            JsonObject payload = newDoc.createNestedObject("payload");
            payload["server"] = settings.server;
            payload["port"] = settings.port;
            payload["login"] = settings.login;
            payload["password"] = settings.password;
            payload["ssl"] = settings.ssl;

            newDoc["status"] = status_ok_message;

            doc = std::move(newDoc);
        }
        break;
    }
    case HTTP_DELETE:
        emailServerSettingsDelete(serverType);
        doc["status"] = status_ok_message;
        break;
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void emailSmtpServerSettingsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    // email_server_type_t serverType = req->pathArg(0).equals("smtp") ? SMTP_EMAIL_SERVER_TYPE : IMAP_EMAIL_SERVER_TYPE;
    email_server_type_t serverType = SMTP_EMAIL_SERVER_TYPE;
    struct EmailServerSettings settings;

    DynamicJsonDocument doc(STRING_JSON_LENGTH);
    JsonObject reqPayload = body.as<JsonObject>();

    strlcpy(settings.server, reqPayload["server"], STRING_LENGTH);
    strlcpy(settings.port, reqPayload["port"], STRING_LENGTH);
    strlcpy(settings.login, reqPayload["login"], STRING_LENGTH);
    strlcpy(settings.password, reqPayload["password"], STRING_LENGTH);
    settings.ssl = reqPayload["ssl"];

    emailServerSettingsSet(serverType, settings);
    doc["status"] = status_ok_message;

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}

void emailImapServerSettingsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    email_server_type_t serverType = IMAP_EMAIL_SERVER_TYPE;
    struct EmailServerSettings settings;

    DynamicJsonDocument doc(STRING_JSON_LENGTH);
    JsonObject reqPayload = body.as<JsonObject>();

    strlcpy(settings.server, reqPayload["server"], STRING_LENGTH);
    strlcpy(settings.port, reqPayload["port"], STRING_LENGTH);
    strlcpy(settings.login, reqPayload["login"], STRING_LENGTH);
    strlcpy(settings.password, reqPayload["password"], STRING_LENGTH);
    settings.ssl = reqPayload["ssl"];

    emailServerSettingsSet(serverType, settings);
    doc["status"] = status_ok_message;

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}

void emailRecipientsGetDeleteHandlerFunction(AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET:
        emailRecipientsJsonEndpoint(GET, reqDoc.to<JsonVariant>(), doc);
        doc["status"] = status_ok_message;
        break;
    case HTTP_DELETE:
        if (!req->pathArg(1).isEmpty()) {
            log_d("removing %s-th email recipients", req->pathArg(1));
            emailRecipientDelete(req->pathArg(1).toInt());
        } else {
            emailRecipientsDeleteAll();
        }
        doc["status"] = status_ok_message;
        break;
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void emailRecipientsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    DynamicJsonDocument doc(STRING_JSON_LENGTH);

    if (body.is<String>()) {
        emailRecipientAdd(body.as<const char *>());
    } else if (body.is<JsonArray>()) {
        JsonArray recipients = body.as<JsonArray>();
        for (int i = 0; i < recipients.size(); i++) {
            emailRecipientAdd(recipients[i]);
        }
    }

    doc["status"] = status_ok_message;

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}


void gsmPinGetDeleteHandlerFunction(AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET:
        char pin[STRING_LENGTH];
        gsmPinGet(pin, STRING_LENGTH);
        doc["payload"] = pin;
        doc["status"] = status_ok_message;
        break;
    case HTTP_DELETE:
        gsmPinDelete();
        doc["status"] = status_ok_message;
        break;
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void gsmPinPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    DynamicJsonDocument doc(STRING_JSON_LENGTH);

    if (body.is<String>()) {
        gsmPinSet(body.as<const char*>());
        doc["status"] = status_ok_message;
    } else {
        doc["status"] = status_error_message;
    }

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}


void gsmRecipientsGetDeleteHandlerFunction(AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET:
        gsmRecipientsJsonEndpoint(GET, reqDoc.to<JsonVariant>(), doc);
        doc["status"] = status_ok_message;
        break;
    case HTTP_DELETE:
        if (!req->pathArg(1).isEmpty()) {
            log_d("removing %s-th gsm recipients", req->pathArg(1));
            gsmRecipientDelete(req->pathArg(1).toInt());
        } else {
            gsmRecipientsDeleteAll();
        }
        doc["status"] = status_ok_message;
        break;
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void gsmRecipientsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    DynamicJsonDocument doc(STRING_JSON_LENGTH);

    if (body.is<String>()) {
        gsmRecipientAdd(body.as<const char *>());
    } else if (body.is<JsonArray>()) {
        JsonArray recipients = body.as<JsonArray>();
        for (int i = 0; i < recipients.size(); i++) {
            gsmRecipientAdd(recipients[i]);
        }
    }

    doc["status"] = status_ok_message;

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}


void gprsNetworkPermGetHanderFunction(AsyncWebServerRequest *req) {

    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET:
        doc["payload"] = gprsPermGet();
        doc["status"] = status_ok_message;
        break;
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void gprsNetworkPermPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    DynamicJsonDocument doc(STRING_JSON_LENGTH);

    if (body["allow"].is<bool>()) {
        gprsPermSet(body["allow"].as<bool>());
        doc["status"] = status_ok_message;
    } else {
        doc["status"] = status_error_message;
    }

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}

void gprsSettingsGetDeleteHanderFunction(AsyncWebServerRequest *req) {
    GprsSettings settings;

    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET:
    {
        error_t err = gprsSettingsGet(settings);
        if (err) {
            doc["status"] = status_error_message;
        } else {
            DynamicJsonDocument newDoc(JSON_DEFAULT_BUFFER_LENGTH + 2 * 3 * STRING_LENGTH);
            JsonObject payload = newDoc.createNestedObject("payload");
            payload["apn"] = settings.apn;
            payload["password"] = settings.password;
            payload["user"] = settings.user;

            doc = std::move(newDoc);
            doc["status"] = status_ok_message;
        }
        break;
    }
    case HTTP_DELETE:
        gprsSettingsDelete();
        doc["status"] = status_ok_message;
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void gprsSettingsPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    DynamicJsonDocument doc(STRING_JSON_LENGTH);
    GprsSettings settings;

    if (body.is<JsonObject>()) {
        strlcpy(settings.apn, body["apn"], STRING_LENGTH);
        strlcpy(settings.password, body["password"], STRING_LENGTH);
        strlcpy(settings.user, body["user"], STRING_LENGTH);
        gprsSettingsSet(settings);

        doc["status"] = status_ok_message;
    } else {
        doc["status"] = status_error_message;
    }

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}

void gsmTestHandlerFunction(AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    
    notifyAboutEvent(TEST_SMS_NOTIFICATION);

    doc["status"] = status_ok_message;
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void gprsEmailTestHandlerFunction(AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    
    notifyAboutEvent(TEST_EMAIL_GPRS_NOTIFICATION);

    doc["status"] = status_ok_message;
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void sensorsBoundariesGetDeleteHanderFunction(AsyncWebServerRequest *req) {
    
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);
    switch (req->method())
    {
    case HTTP_GET: {
        int h, l;
        JsonObject payload = doc.createNestedObject("payload");
        JsonObject waterSensor = payload.createNestedObject("waterSensor");
        getWaterSensorBoundaries(l, h);
        waterSensor["low"] = l;
        waterSensor["high"] = h;
        JsonObject tempSensor = payload.createNestedObject("tempSensor");
        getTemperatureBoundaries(l, h);
        tempSensor["low"] = l;
        tempSensor["high"] = h;
        JsonObject humidSensor = payload.createNestedObject("humidSensor");
        getHumidityBoundaries(l, h);
        humidSensor["low"] = l;
        humidSensor["high"] = h;
        doc["status"] = status_ok_message;
        break;
    }
    default:
        doc["status"] = status_error_message;
        break;
    }
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}

void sensorsBoundariesPostHandlerFunction(AsyncWebServerRequest *req, JsonVariant &body) {
    DynamicJsonDocument doc(STRING_JSON_LENGTH);
    GprsSettings settings;

    if (body.is<JsonObject>()) {
        JsonObject waterSensor = body["waterSensor"];
        setWaterSensorBoundaries(waterSensor["low"], waterSensor["high"]);
        JsonObject tempSensor = body["tempSensor"];
        int l, h;
        l = tempSensor["low"];
        h = tempSensor["high"];
        log_d("temp bounds: %d %d", l, h);
        setTemperatureBoundaries(l, h);
        JsonObject humidSensor = body["humidSensor"];
        l = humidSensor["low"];
        h = humidSensor["high"];
        log_d("humid bounds: %d %d", l, h);
        setHumidityBoundaries(l, h);
        doc["status"] = status_ok_message;
    } else {
        doc["status"] = status_error_message;
    }

    AsyncResponseStream *res = req->beginResponseStream("application/json");
    res->setCode(200);

    serializeJson(doc, *res);

    req->send(res);
}

void sensorsValuesGetHandlerFunction(AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(JSON_DEFAULT_BUFFER_LENGTH);
    DynamicJsonDocument reqDoc(0);

    SensorsValues values;
    getSensorsValues(values);

    JsonObject payload = doc.createNestedObject("payload");
    payload["waterSensor"] = values.water;
    payload["tempSensor"] = values.temperature;
    payload["humidSensor"] = values.humidity;

    doc["status"] = status_ok_message;
    
    AsyncResponseStream *res = req->beginResponseStream("application/json");
    serializeJson(doc, *res);

    res->setCode(200);
    req->send(res);
}