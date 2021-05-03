#ifndef _COMMAND_CONTROLLER_H_
#define _COMMAND_CONTROLLER_H_

#include <stddef.h>

char *processMessage(const char *message, size_t messageLen, char *outputBuffer, size_t outputLen);

#endif