#pragma once

#ifndef _INTERRUPT_CONTROLLER_H_
#define _INTERRUPT_CONTROLLER_H_

#include <MemoryController.h>
#include <WifiController.h>

void BindInterrupts(bool stateIsConfig);
void ProcessInterrupts(bool stateIsConfig);

#endif