#ifndef _INTERRUPT_CONTROLLER_H_
#define _INTERRUPT_CONTROLLER_H_

#include <MemoryController.h>
#include <EmailService.h>
#include <IOController.h>

void initButtonsInterrupts(bool stateIsConfig);
void processButtons();
void unbindInterrupts();
void rebindInterrupts();
#endif