#ifndef _JSON_TERMINAL_H_
#define _JSON_TERMINAL_H_

#include <ArduinoJson.h>

char *processJsonMessage(const char *req, size_t reqLen, char *outputBuffer, size_t outputLen, error_t &parsingError);

#endif