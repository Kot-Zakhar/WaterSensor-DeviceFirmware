#pragma once

#ifndef _INTERRUPT_CONTROLLER_H_
#define _INTERRUPT_CONTROLLER_H_

#include <MemoryController.h>
#include <WifiController.h>
#include <IOController.h>

#define RESET_BUTTON_PIN 0
#define MODE_BUTTON_PIN 2
#define MESSAGE_BUTTON_PIN 16
#define ANY_BUTTON_PIN 14

#define POTENCIOMETR_PIN 39

void BindInterrupts(bool stateIsConfig);
void ProcessInterrupts(bool stateIsConfig);

#endif