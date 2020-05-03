#ifndef _BLUETOOTH_CONTROLLER_H_
#define _BLUETOOTH_CONTROLLER_H_

#include <MemoryController.h>
#include <WifiController.h>
#include <IOController.h>

#include <BtCommands.h>
#include <BtIO.h>

#include <BtTerminal.h>
#include <BtJson.h>

void InitBtController();
void ProcessBt();
int ReadBtCommand(char *buffer, int maxLength);

#endif