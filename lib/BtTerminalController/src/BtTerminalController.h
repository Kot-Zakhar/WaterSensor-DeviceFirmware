#pragma once
#ifndef _BLUETOOTH_TERMINAL_CONTROLLER_H_
#define _BLUETOOTH_TERMINAL_CONTROLLER_H_

#include "BluetoothSerial.h"
#include <MemoryController.h>
#include <WifiController.h>

void InitBtTerminalController();
void PrintNetworksFromMemoryCommand();

void ProcessBt();

#endif