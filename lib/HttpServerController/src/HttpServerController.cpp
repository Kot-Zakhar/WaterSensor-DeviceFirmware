#include "HttpServerController.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LITTLEFS.h>

// WiFiServer httpServer;
AsyncWebServer server(80);

void initHttpServer() {
    // httpServer.begin(80);

    server
        .serveStatic("/", LITTLEFS, "/public")
        .setDefaultFile("/index.html");
    server.on("/api", HTTP_GET | HTTP_POST | HTTP_DELETE, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Hello world");
    });
    server.begin();
}

void processHttpServer() {
    
    // WiFiClient client = httpServer.available();

    // if (client) {
    //     Serial.printf("%s\n", client.remoteIP().toString().c_str());

    //     char *req = (char *)malloc(client.available());

    //     client.readBytes(req, client.available());
    //     Serial.printf("\"%s\"\n", req);

    //     char *method = req;

    //     char *path = req;
    //     strsep(&path, " ");

    //     char *protocol = path;
    //     strsep(&protocol, " ");

    //     char *otherHeaders = protocol;
    //     strsep(&otherHeaders, "\r\n");
        
    //     client.println("HTTP/1.1 200 OK");
    //     client.println("Content-type:text/html");
    //     client.println("Connection: close");
    //     client.println();
        
    //     File f = LITTLEFS.open("/index.html");
    //     while(f.available()) {
    //         client.write(f.read());
    //     }
    //     f.close();

    //     client.printf("method: %s\n", method);
    //     client.printf("path: %s\n", path);
    //     client.printf("protocol: %s\n", protocol);
    //     client.println();

    //     client.stop();

    //     free(req);
    // }
}