#ifndef BLUETOOTH_IO_H
#define BLUETOOTH_IO_H

#include <BluetoothSerial.h>
// #include <IOController.h>

void initBtIO(const char *name);

void writeBt(const char* line);
int readBt(char *buffer, int maxLength);
int awaitAndReadBt(char *buffer, int maxLength);
BluetoothSerial *getCurrentBtSerial();
int btAvailable();

#endif