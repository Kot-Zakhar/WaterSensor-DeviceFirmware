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

// AsyncCallbackWebHandler* wifiCredsWebHandler = new AsyncCallbackWebHandler();

void initHttpServer() {        
    server.on("^\\/api\\/wifi-creds(\\/([0-9]*))?$", HTTP_GET | HTTP_DELETE , [](AsyncWebServerRequest *req) {
        DynamicJsonDocument doc(STRING_JSON_LENGTH);
        DynamicJsonDocument reqDoc(0);
        if (req->method() & HTTP_GET) {
            wifiCredsJsonEndpoint(GET, reqDoc.to<JsonVariant>(), doc);
        } else if (req->method() & HTTP_DELETE) {

            if (!req->pathArg(1).isEmpty()) {
                log_d("removing %s-th record", req->pathArg(1));
                wifiCredsDelete(req->pathArg(1).toInt());
            } else {
                wifiCredsDeleteAll();
            }
        }

        doc["status"] = status_ok_message;
        
        AsyncResponseStream *res = req->beginResponseStream("application/json");
        serializeJson(doc, *res);

        res->setCode(200);
        req->send(res);
    });

    server.addHandler(new AsyncCallbackJsonWebHandler("/api/wifi-creds", [](AsyncWebServerRequest *req, JsonVariant &body) {
        AsyncResponseStream *res = req->beginResponseStream("application/json");
        DynamicJsonDocument doc(STRING_JSON_LENGTH);
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
    }));




    server
        .serveStatic("/", LITTLEFS, "/public")
        .setDefaultFile("/index.html");

    server.begin();
}
