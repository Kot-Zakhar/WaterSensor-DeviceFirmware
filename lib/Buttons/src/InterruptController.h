#ifndef _INTERRUPT_CONTROLLER_H_
#define _INTERRUPT_CONTROLLER_H_

#include <MemoryController.h>
#include <EmailController.h>
#include <IOController.h>

#define POTENCIOMETR_PIN 39

void bindInterrupts(bool stateIsConfig);
void processInterrupts();
void unbindInterrupts();
void rebindInterrupts();
#endif