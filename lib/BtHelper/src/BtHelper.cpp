#include "BtHelper.h"

char* WIFI_SSID_AND_PASSWORD_LINE =  "new";
char* WIFI_ERASE_MEMORY_LINE =  "clear";
char* WIFI_SHOW_NETWORKS_LINE = "show";
char* RESTART_LINE = "restart";

char* commands[] = {
    "", 
    WIFI_SSID_AND_PASSWORD_LINE, 
    WIFI_ERASE_MEMORY_LINE, 
    WIFI_SHOW_NETWORKS_LINE,
    RESTART_LINE
};

int ReadBtLine(BluetoothSerial *SerialBT, String *buffer, int maxLength){
    if (!SerialBT->available() || buffer->length() <= 0)
        return -1;

    int index = 0;

    char character = 0;
    buffer->clear();

    while (index < maxLength - 1){
        character = (char)SerialBT->read();
        if ((character == '\n') || (character == '\r'))
            break;
        *buffer = *buffer + character;
        index++;
    } 
    Serial.println(*buffer);

    while (SerialBT->available())
        SerialBT->read();

    return buffer->length();
}

int ReadBtCommand(BluetoothSerial *SerialBT, String *buffer, int maxLength){
    if (!SerialBT->available())
        return -1;
    
    int symbolsRead = ReadBtLine(SerialBT, buffer, maxLength);
    
    if (symbolsRead < 0)
        return -1;

    for (int j = 1; j <= COMMAND_AMOUNT; j++){
        if (!buffer->compareTo(String(commands[j])))
            return j;
    }
    
    return 0;
}

void WriteBtLine(BluetoothSerial *SerialBT, const char* line){
  SerialBT->write((const uint8_t *)line, strlen(line));
  SerialBT->write('\n');
//   Serial.println("WritebtLine(");
  Serial.println(line);
}