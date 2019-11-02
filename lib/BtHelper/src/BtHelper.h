#pragma once
#ifndef _BLUETOOTH_HELPER_H_
#define _BLUETOOTH_HELPER_H_

#define COMMAND_AMOUNT 4

#define COMMAND_NOP 0
#define WIFI_SSID_AND_PASSWORD 1
#define WIFI_ERASE_MEMORY 2
#define WIFI_SHOW_NETWORKS 3
#define RESTART 4

#include "BluetoothSerial.h"

int ReadBtLine(BluetoothSerial *SerialBT, String *buffer, int maxLength);
int ReadBtCommand(BluetoothSerial *SerialBT, String *buffer, int maxLength);
void WriteBtLine(BluetoothSerial *SerialBT, const char* line);

#endif