#ifndef _TEXT_TERMINAL_H_
#define _TEXT_TERMINAL_H_

#include <stddef.h>

char *processTextMessage(const char *message, size_t messageLen, char *outputBuffer, size_t outputLen);

#endif