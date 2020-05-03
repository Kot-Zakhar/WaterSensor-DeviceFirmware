#ifndef BLUETOOTH_IO_H
#define BLUETOOTH_IO_H

#include <BluetoothSerial.h>
#include <IOController.h>

void InitBtIO(const char *name);

void WriteBt(const char* line);
int ReadBt(char *buffer, int maxLength);
int AwaitAndReadBt(char *buffer, int maxLength);
BluetoothSerial *GetCurrentBtSerial();

#endif