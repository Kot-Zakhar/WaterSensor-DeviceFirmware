#include <BtController.h>

const char* bt_credentials_name = "ESP32";

void InitBtController(){
    InitBtIO(bt_credentials_name);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, ("BT - " + String(bt_credentials_name)).c_str());
}

void ProcessBt() {

    char *commandLine = (char *)malloc(STRING_LENGTH);

    int command = ReadBtCommand(commandLine, STRING_LENGTH);

    if (command >= 0) {

      if (command > JSON_BT_COMMANDS) {
        ProcessBtJsonCommand(command);
      } else {
        IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, ("Command: " + String(commands[command]) + ".").c_str());
        ProcessBtTerminalCommand(command);
      }
    }
    
    free(commandLine);
}

int ReadBtCommand(char *buffer, int maxLength){
  
  if (ReadBt(buffer, maxLength) == -1)
    return NOT_AVAILABLE;
  
  String line = String(buffer);

  bool isJson = line.indexOf(json_prefix) == 0;

  if (isJson) {
    line = line.substring(strlen(json_prefix));
  }

  int commandIndex;

  for (commandIndex = 0; commandIndex < COMMAND_AMOUNT; commandIndex++){
    if (!line.compareTo(String(commands[commandIndex])))
      break;
  }

  if (commandIndex == COMMAND_AMOUNT) {
    log_e("Not recognized command");
    return NOT_RECOGNISED;
  }

  if (isJson) {
    log_i("Json command");
    commandIndex += JSON_BT_COMMAND;
  }

  return commandIndex;
  
}