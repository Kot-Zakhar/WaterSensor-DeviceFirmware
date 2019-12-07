#pragma once

#ifndef _INTERRUPT_CONTROLLER_H_
#define _INTERRUPT_CONTROLLER_H_

#include <MemoryController.h>
#include <WifiController.h>
#include <IOController.h>

#define POTENCIOMETR_PIN 39

void BindInterrupts(bool stateIsConfig);
void ProcessInterrupts();

#endif