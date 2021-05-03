#include "CommandController.h"

#include "JsonTerminal.h"
#include "TextTerminal.h"

char *processMessage(const char *message, size_t messageLen, char *outputBuffer, size_t outputLen) {

  error_t parsingError = 0;
  log_d("Trying to process message as json.");
  outputBuffer = processJsonMessage(message, messageLen, outputBuffer, outputLen, parsingError);
  if (parsingError) {
    log_d("Failed json parsing: processing as text command.");
    outputBuffer = processTextMessage(message, messageLen, outputBuffer, outputLen);
  }

  return outputBuffer;
}