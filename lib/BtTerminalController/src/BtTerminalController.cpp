#include <BtTerminalController.h>

BluetoothSerial BT;

const char* bt_credentials_name = "ESP32";

enum bt_command_t {
  NOT_AVAILABLE = -2,
  NOT_RECOGNISED,
  BT_PING,
  WIFI_SSID_AND_PASSWORD,
  WIFI_ERASE,
  WIFI_SHOW_NETWORKS,
  SMTP_SETTINGS,
  RESTART,
  SWITCH_MODE,
  HELP,
  COMMAND_AMOUNT,
};

// commands
static const char* commands[] = {
  "ping",
  "new wifi",
  "clear wifi",
  "show",
  "smtp",
  "restart",
  "switch mode",
  "help"
};

// bt messages
static const char *wifi_ssid_request_message = "Provide ssid of network:";
static const char *wifi_ssid_confirmation_message = "Ssid received.";
static const char *wifi_password_request_message = "Provide passowrd of network:";
static const char *wifi_password_confirmation_message = "Password received.";
static const char *erasing_wifi_credentials_message = "Erasing wifi credentials.";
static const char *status_ok_message = "Ok.";
static const char *status_error_message = "Error.";
static const char *unknown_command_message = "Unknown command.";
static const char *restart_message = "Restarting...";
static const char *pong_message = "pong";
static const char *memory_empty_message = "No networks in memory";


void PingCommand();
void AddWiFiCredentialsCommand();
void ClearWiFiCredentialsCommand();
void PrintNetworksFromMemoryCommand();
void SmtpConfigureCommand();
void RestartESPCommand();
void SwitchModeCommand();
void HelpCommand();

void WriteBtLine(const char* line);
int ReadBtLine(char *buffer, int maxLength);
int ReadBtCommand(char *buffer, int maxLength);
int AwaitAndReadBtLine(char *buffer, int maxLength);
void BtInterruptCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

void InitBtTerminalController(){
  BT.begin(bt_credentials_name);
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, ("BT - " + String(bt_credentials_name)).c_str());
  BT.register_callback(BtInterruptCallback);
}

void ProcessBt(){
  if (!BT.available())
    return;

  char *commandLine = (char *)malloc(STRING_LENGTH);

  int command = ReadBtCommand(commandLine, STRING_LENGTH);

  IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, ("Command: " + String(commands[command]) + ".").c_str());

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
  case WIFI_ERASE:
    ClearWiFiCredentialsCommand();
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
    WriteBtLine(unknown_command_message);
    break;
  }

  free(commandLine);
}

void PingCommand(){
  WriteBtLine(pong_message);
}

void AddWiFiCredentialsCommand(){
  IOIndicate(BT_PENDING_COMMAND);

  char *ssid = (char *)malloc(STRING_LENGTH);
  char *password = (char *)malloc(STRING_LENGTH);
    
  WriteBtLine(wifi_ssid_request_message);
  if (AwaitAndReadBtLine(ssid, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  Serial.println(wifi_ssid_confirmation_message);

  WriteBtLine(wifi_password_request_message);
  if (AwaitAndReadBtLine(password, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  Serial.println(wifi_password_confirmation_message);

  int newAmount = SaveWiFiCredentialsInMemory(ssid, password);

  WriteBtLine(status_ok_message);
  IOWrite(IO_WRITE_SCREEN, (String(ssid) + " saved.").c_str());
  IOWrite(IO_WRITE_SCREEN, (String(newAmount) + " networks in memory.").c_str());
  IOIndicate(BT_END_COMMAND);

  free(ssid);
  free(password);
}

void PrintNetworksFromMemoryCommand(){
  int counter = GetWiFiCredentialsAmountFromMemory();

  if (counter == 0){
      WriteBtLine(memory_empty_message);
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

    for (int i = 0; i < counter; i++){
      GetWiFiSsidFromMemory(i, ssid);
      GetWiFiPasswordFromMemory(i, password);
      String output = String(i + 1) + ":'" + String(ssid) + "'-'" + String(password) + "'";
      WriteBtLine(output.c_str());
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
  WriteBtLine(helpMessage.c_str());

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

    WriteBtLine("Write a command in a following way:\n[command number] [value]");    
    if (AwaitAndReadBtLine(buffer, STRING_LENGTH) < 0){
      WriteBtLine(status_error_message);
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
    WriteBtLine(response.c_str());
  } while (command != 0);
  free(buffer);
  ClearDisplay();
  IOIndicate(BT_END_COMMAND);
}

void ClearWiFiCredentialsCommand(){
  WriteBtLine(erasing_wifi_credentials_message);
  ClearWiFiCredentials();
  IOWrite(IO_WRITE_SCREEN, erasing_wifi_credentials_message);
}

void RestartESPCommand(){
  WriteBtLine(restart_message);
  ESP.restart();
}

void SwitchModeCommand(){
  IOIndicate(BT_PENDING_COMMAND);
  bool mode = IsConfigStateInMemory();
  String currentMode = String("Mode ") + (mode ? "`configuration`" : "`working`");
  WriteBtLine(currentMode.c_str());
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, currentMode.c_str());

  String answer;
  char *buffer = (char *)malloc(STRING_LENGTH);
  do {
    WriteBtLine((String("Would you like to switch it to ") + (!mode ? "`configuration`" : "`working`") + "? (y/n)").c_str());
    AwaitAndReadBtLine(buffer, STRING_LENGTH);
    answer = String(buffer);
    answer.toLowerCase();
  } while (answer.compareTo("y") && answer.compareTo("n"));
  if (!answer.compareTo("y")){
    SetStateInMemory(!mode);
    WriteBtLine(status_ok_message);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, (String("New mode ") + (!mode ? "`configuration`" : "`working`")).c_str());
  } else {
    WriteBtLine("Aborting.");
  }
  free(buffer);
  IOIndicate(BT_END_COMMAND);
}

void HelpCommand(){
  IOIndicate(BT_PENDING_COMMAND);
  WriteBtLine("List of commands:");
  String message;
  for (int i = 0; i < COMMAND_AMOUNT; i++){
    message += String(commands[i]) + "\n";
  }
  WriteBtLine(message.c_str());
  IOIndicate(BT_END_COMMAND);
}

// IO

int ReadBtCommand(char *buffer, int maxLength){
  if (!BT.available())
      return NOT_AVAILABLE;
  
  ReadBtLine(buffer, maxLength);
  
  String line = String(buffer);

  for (int j = 0; j < COMMAND_AMOUNT; j++){
    if (!line.compareTo(String(commands[j])))
      return j;
  }
  
  return NOT_RECOGNISED;
}

int AwaitAndReadBtLine(char *buffer, int maxLength){
  while (!BT.available()){}
  return ReadBtLine(buffer, maxLength);
}

int ReadBtLine(char *buffer, int maxLength){
  if (!BT.available())
      return -1;

  int charAmount = BT.readBytes(buffer, maxLength);

  
  for (int i = 0; i < charAmount; i++){
    if ((buffer[i] == '\n') || (buffer[i] == '\r')){
      buffer[i] = 0;
      charAmount = i;
      break;
    }
  }

  return charAmount;
}

void WriteBtLine(const char* line){
  BT.write((const uint8_t *)line, strlen(line));
  BT.write('\n');
  Serial.println(line);
  delay(100);
}

// Interrupts

void BtInterruptCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if (event == ESP_SPP_SRV_OPEN_EVT){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Connected to bt.");
    IOIndicate(BT_CONNECTED);
  }
  if (event == ESP_SPP_CLOSE_EVT){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Disconnected from bt.");
    IOIndicate(BT_DISCONNECTED);
  }
}