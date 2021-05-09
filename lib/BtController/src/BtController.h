#ifndef _BLUETOOTH_CONTROLLER_H_
#define _BLUETOOTH_CONTROLLER_H_

#include <MemoryController.h>
#include <WifiController.h>
#include <IOController.h>

#include <BtIO.h>

#include <CommandController.h>

void initBtController();

bool btNeedToBeProcessed();
void processBt();

#endif