#include <BtTerminal.h>

void PingCommand();
void AddWiFiCredentialsCommand();
void RemoveAllWiFiCredentialsCommand();
void RemoveSingleWiFiCredentialCommand();
void PrintNetworksFromMemoryCommand();
void SmtpConfigureCommand();
void RestartESPCommand();
void SwitchModeCommand();
void HelpCommand();


void ProcessBtTerminalMessage(const char* message, int length) {
  int commandIndex;

  for (commandIndex = 0; commandIndex < COMMAND_AMOUNT; commandIndex++){
    log_d("Comparing two strings: actual command '%s' vs template '%s'", message, commands[commandIndex]);
    if (!strcmp(message, commands[commandIndex]))
      break;
  }

  if (commandIndex == COMMAND_AMOUNT)
    commandIndex = NOT_RECOGNISED;

  ProcessBtTerminalCommand(commandIndex);
}


void ProcessBtTerminalCommand(int command){
  if (command < 0){
    IOIndicate(BT_GOT_BAD_COMMAND);
  } else {
    IOIndicate(BT_GOT_GOOD_COMMAND);
  }
  
  switch (command)
  {
  case BT_PING:
    PingCommand();
    break;
  case WIFI_SSID_AND_PASSWORD:
    AddWiFiCredentialsCommand();
    break;
  case WIFI_REMOVE_ALL:
    RemoveAllWiFiCredentialsCommand();
    break;
  case WIFI_REMOVE_SINGLE:
    RemoveSingleWiFiCredentialCommand();
    break;
  case WIFI_SHOW_NETWORKS:
    PrintNetworksFromMemoryCommand();
    break;
  case SMTP_SETTINGS:
    SmtpConfigureCommand();
    break;
  case RESTART:
    RestartESPCommand();
    break;
  case SWITCH_MODE:
    SwitchModeCommand();
    break;
  case HELP:
    HelpCommand();
    break;
  case NOT_RECOGNISED:
    WriteBt(unknown_command_message);
    break;
  default:
    WriteBt(not_supported_command_message);
    break;
  }
}

void PingCommand(){
  WriteBt(pong_message);
}

void AddWiFiCredentialsCommand(){
  IOIndicate(BT_PENDING_COMMAND);

  char *ssid = (char *)malloc(STRING_LENGTH);
  char *password = (char *)malloc(STRING_LENGTH);
    
  WriteBt(wifi_ssid_request_message);
  if (AwaitAndReadBt(ssid, STRING_LENGTH) < 0){
    WriteBt(status_error_message);
    return;
  }
  Serial.println(wifi_ssid_confirmation_message);

  WriteBt(wifi_password_request_message);
  if (AwaitAndReadBt(password, STRING_LENGTH) < 0){
    WriteBt(status_error_message);
    return;
  }
  Serial.println(wifi_password_confirmation_message);

  int newAmount = SaveWiFiCredentialsInMemory(ssid, password);

  WriteBt(status_ok_message);
  IOWrite(IO_WRITE_SCREEN, (String(ssid) + " saved.").c_str());
  IOWrite(IO_WRITE_SCREEN, (String(newAmount) + " networks in memory.").c_str());
  IOIndicate(BT_END_COMMAND);

  free(ssid);
  free(password);
}

void PrintNetworksFromMemoryCommand(){
  int counter = GetWiFiCredentialsAmountFromMemory();

  if (counter == 0){
      WriteBt(memory_empty_message);
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

    for (int i = 0; i < counter; i++){
      GetWiFiSsidFromMemory(i, ssid);
      GetWiFiPasswordFromMemory(i, password);
      String output = String(i + 1) + ":'" + String(ssid) + "'-'" + String(password) + "'";
      WriteBt(output.c_str());
      if (i < MAX_LINES_AMOUNT - 1){
        IOWrite(IO_WRITE_SCREEN, output.c_str());
      }
    }

    free(ssid);
    free(password);
  }
}

void SmtpConfigureCommand(){
  IOIndicate(BT_PENDING_COMMAND);
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, "Configuring smtp.");
  // todo: Unbind hardcoded index of command to smtp value index
  String helpMessage = "0 - exit;\n1 - show all settings;\n";
  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++)
    helpMessage += (i + 2) + String(" [value] - set ") + email_settings[i] + " to 'value';\n";
  WriteBt(helpMessage.c_str());

  String commandLine;
  int command;
  String response;
  String currentSettings;
  char *buffer = (char *)malloc(STRING_LENGTH);

  do {
    currentSettings = String("Smtp settings:");
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, currentSettings.c_str());
    for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++){
      response = GetEmailValue(i, buffer);
      if (i != EMAIL_PASS && i != EMAIL_IMAP_PORT && i != EMAIL_SMTP_PORT)
        IOWrite(IO_WRITE_SCREEN, response.c_str());
      currentSettings = email_settings[i] + String(": ") + currentSettings + "\n" + response;
    }

    WriteBt("Write a command in a following way:\n[command number] [value]");    
    if (AwaitAndReadBt(buffer, STRING_LENGTH) < 0){
      WriteBt(status_error_message);
      return;
    }

    commandLine = String(buffer);
    command = commandLine.toInt();
    commandLine = commandLine.substring(2);
    Serial.println("Command: " + String(command));
    Serial.println("Value: " + String(commandLine));
    switch (command){
      case 0:
        response = String("Exiting.");
        break;
      case 1:
        response = String("Smtp settings:");
        for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++){
          // todo: check for setting existance
          response = response + "\n" + email_settings[i] + ": " + GetEmailValue(i, buffer);
        }
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        SetEmailValue(EMAIL_SMTP_SERVER + command - 2, commandLine.c_str());
        response = String(status_ok_message);
        break;
      default:
        response = String("Command not recognize.");
    }
    WriteBt(response.c_str());
  } while (command != 0);
  free(buffer);
  ClearDisplay();
  IOIndicate(BT_END_COMMAND);
}

void RemoveAllWiFiCredentialsCommand(){
  WriteBt(erasing_wifi_credentials_message);
  RemoveAllWiFiCredentials();
  IOWrite(IO_WRITE_SCREEN, erasing_wifi_credentials_message);
}

void RemoveSingleWiFiCredentialCommand() {
  WriteBt(specify_wifi_index_or_uuid_message);
  char *response = (char *) malloc(STRING_LENGTH);
  AwaitAndReadBt(response, STRING_LENGTH);
  int index;
  int removed;
  if (sscanf(response, "%d", &index)) {
    removed = RemoveWiFiCredentials(index);
  } else {
    removed = RemoveWiFiCredentials(response);
  }

  log_d("%d wifi credentials '%s' are removed.", removed, response);

  IOWrite(IO_WRITE_SCREEN, erasing_wifi_credentials_message);

  free(response);
}

void RestartESPCommand(){
  WriteBt(restart_message);
  ESP.restart();
}

void SwitchModeCommand(){
  IOIndicate(BT_PENDING_COMMAND);
  bool mode = IsConfigStateInMemory();
  String currentMode = String("Mode ") + (mode ? "`configuration`" : "`working`");
  WriteBt(currentMode.c_str());
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, currentMode.c_str());

  String answer;
  char *buffer = (char *)malloc(STRING_LENGTH);
  do {
    WriteBt((String("Would you like to switch it to ") + (!mode ? "`configuration`" : "`working`") + "? (y/n)").c_str());
    AwaitAndReadBt(buffer, STRING_LENGTH);
    answer = String(buffer);
    answer.toLowerCase();
  } while (answer.compareTo("y") && answer.compareTo("n"));
  if (!answer.compareTo("y")){
    SetStateInMemory(!mode);
    WriteBt(status_ok_message);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, (String("New mode ") + (!mode ? "`configuration`" : "`working`")).c_str());
  } else {
    WriteBt("Aborting.");
  }
  free(buffer);
  IOIndicate(BT_END_COMMAND);
}

void HelpCommand(){
  IOIndicate(BT_PENDING_COMMAND);
  WriteBt("List of commands:");
  String message;
  for (int i = 0; i < COMMAND_AMOUNT; i++){
    message += String(commands[i]) + "\n";
  }
  WriteBt(message.c_str());
  IOIndicate(BT_END_COMMAND);
}
