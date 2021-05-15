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

AsyncWebServer server(80);

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