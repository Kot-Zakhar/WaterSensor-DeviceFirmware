#ifndef _BLUETOOTH_TERMINAL_CONTROLLER_H_
#define _BLUETOOTH_TERMINAL_CONTROLLER_H_

#include <IOController.h>
#include <BtCommands.h>
#include <BtIO.h>
#include <MemoryController.h>

void processBtTerminalCommand(int command);
void processBtTerminalMessage(const char* message, int length);



#endif