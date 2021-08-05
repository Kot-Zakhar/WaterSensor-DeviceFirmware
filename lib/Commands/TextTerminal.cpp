#include "SimpleCLI.h"

#include "TextTerminal.h"
#include "CommandHandlers.h"
#include "Commands.h"
#include "ResponseMessages.h"
#include "Scheduler.h"

SimpleCLI cli;

char *resultMessage = nullptr;
int resultMessageLen = 0;

void registerPingCommand();
void registerWifiCredsCommand();
void registerEmailSmtpCommand();
void registerEmailRecipientsCommand();
void registerModeCommand();
void registerRestartCommand();

void initTextTerminal() {
    registerPingCommand();
    registerWifiCredsCommand();
    registerEmailSmtpCommand();
    registerEmailRecipientsCommand();
    registerModeCommand();
    registerRestartCommand();
}

char *processTextMessage(const char *message, size_t messageLen, char *outputBuffer, size_t outputLen) {
    log_d("message: \"%s\" len: %d", message, messageLen);
    String msg(message);
    msg.trim();
    log_d("new message: \"%s\" len %d", msg.c_str(), msg.length());
    cli.parse(msg);
    log_d("parsed");
    if (cli.errored()) {
        CommandError e = cli.getError();
        outputBuffer = strcpy(outputBuffer, status_error_message);
        log_d("error: %s", e.toString().c_str());
        // sprintf(outputBuffer, "%s %s", status_error_message, e.toString().c_str());
    } else {
        if (resultMessage) {
            outputBuffer = (char *)realloc(outputBuffer, resultMessageLen);
            outputBuffer = strcpy(outputBuffer, resultMessage);
            free(resultMessage);
            resultMessage = nullptr;
            resultMessageLen = 0;
        } else {
            sprintf(outputBuffer, status_ok_message);
        }
    }

    return outputBuffer;
}

void registerPingCommand() {
    cli.addCmd(commands[PING_COMMAND], [](cmd *commandPointer) {
        log_d("cli ping command");
    });
}

command_method_t getMethod(cmd *commandPointer) {
    Command cmd(commandPointer);
    Argument method = cmd.getArg("method");
    String methodValue = method.getValue();
    methodValue.toLowerCase();
    int methodIndex;
    for (methodIndex = 0; methodIndex < int(METHOD_AMOUNT); methodIndex ++)
        if (!methodValue.compareTo(commandMethods[methodIndex]))
            break;
    if (methodIndex == int(METHOD_AMOUNT))
        methodIndex = int(NO_METHOD);
    return (command_method_t)methodIndex;
}

void registerWifiCredsCommand() {
    Command command = cli.addCmd(commands[WIFI_CREDS_COMMAND], [](cmd *commandPointer) {
        Command cmd(commandPointer);
        command_method_t method = getMethod(commandPointer);
        if (method == NO_METHOD) {
            resultMessage = (char *) malloc(STRING_LENGTH);
            resultMessageLen = STRING_LENGTH;
            sprintf(resultMessage, "%s Method is wrong", status_error_message);
            return;
        }
        String ssid = cmd.getArg("ssid").getValue();
        String pass = cmd.getArg("password").getValue();
        switch (method)
        {
        case GET: {
            int amount = wifiCredsGetAmount();
            if (!amount) {
                resultMessage = (char *) malloc(STRING_LENGTH);
                resultMessageLen = STRING_LENGTH;
                resultMessage = strcpy(resultMessage, "No records");
            } else {
                struct WiFiCred* credsBuffer = (struct WiFiCred*)malloc(amount * sizeof(struct WiFiCred));
                credsBuffer = wifiCredsGetAll(credsBuffer, amount);
                resultMessageLen = STRING_LENGTH + amount * sizeof(struct WiFiCred);
                resultMessage = (char *) malloc(resultMessageLen);
                char *resultMessagePtr = resultMessage;
                for (int i = 0; i < amount; i++) {
                    resultMessagePtr += sprintf(resultMessagePtr, "%s\"%s\" \"%s\"",
                        i == 0 ? "" : "\n",
                        credsBuffer[i].ssid,
                        credsBuffer[i].password
                    );
                }

                free(credsBuffer);
            }
        }
            break;
        case POST:
            if (ssid.isEmpty()) {
                resultMessageLen = STRING_LENGTH;
                resultMessage = (char *)malloc(resultMessageLen);
                sprintf(resultMessage, "%s Ssid should not be empty", status_error_message);
            } else {
                wifiCredsAdd(ssid.c_str(), pass.c_str());
            }
            break;
        case DELETE:
            if (ssid.isEmpty()) {
                resultMessageLen = STRING_LENGTH;
                resultMessage = (char *)malloc(resultMessageLen);
                sprintf(resultMessage, "%s Ssid should not be empty", status_error_message);
            } else {
                wifiCredsDelete(ssid.c_str());
            }
            break; 
        default:
            break;
        }
    });
    command.addArg("method", "get");
    command.addPositionalArgument("s/sid", "");
    command.addPositionalArgument("p/assword", "");
}

void registerEmailSmtpCommand() {
    Command command = cli.addCmd(commands[EMAIL_SMTP_COMMAND], [](cmd *commandPointer) {
        Command cmd(commandPointer);
        command_method_t method = getMethod(commandPointer);
        if (method == NO_METHOD) {
            resultMessage = (char *) malloc(STRING_LENGTH);
            resultMessageLen = STRING_LENGTH;
            sprintf(resultMessage, "%s Method is wrong", status_error_message);
            return;
        }

        String server = cmd.getArg("server").getValue();
        String port = cmd.getArg("port").getValue();
        String login = cmd.getArg("login").getValue();
        String password = cmd.getArg("password").getValue();
        String ssl = cmd.getArg("ssl").getValue();
        ssl.toLowerCase();

        EmailServerSettings settings;
        switch (method)
        {
        case GET: {
            error_t error = emailServerSettingsGet(SMTP_EMAIL_SERVER_TYPE, settings);
            if (error) {
                resultMessage = (char *) malloc(STRING_LENGTH);
                resultMessageLen = STRING_LENGTH;
                sprintf(resultMessage, "%s SMTP settings are not set", status_error_message);
            } else {
                resultMessage = (char *) malloc(STRING_LENGTH * 5);
                resultMessageLen = STRING_LENGTH * 5;
                sprintf(resultMessage,
                    "server: \"%s\"\n"
                    "port: \"%s\"\n"
                    "login: \"%s\"\n"
                    "password: \"%s\"\n"
                    "ssl: \"%s\"",
                    settings.server,
                    settings.port,
                    settings.login,
                    settings.password,
                    settings.ssl ? "true" : "false"    
                );
            }
        }
            break;
        case POST:
            if (server.isEmpty() || port.isEmpty() || login.isEmpty() || password.isEmpty() || ssl.isEmpty()) {
                resultMessageLen = STRING_LENGTH;
                resultMessage = (char *)malloc(resultMessageLen);
                sprintf(resultMessage, "%s All fields should be specified", status_error_message);
            } else {
                strcpy(settings.server, server.c_str());
                strcpy(settings.port, port.c_str());
                strcpy(settings.login, login.c_str());
                strcpy(settings.password, password.c_str());
                settings.ssl = ssl.equals("true");
                emailServerSettingsSet(SMTP_EMAIL_SERVER_TYPE, settings);
            }
            break;
        case DELETE:
            emailServerSettingsDelete(SMTP_EMAIL_SERVER_TYPE);
            break; 
        default:
            break;
        }
    });
    command.addArg("method", "get");
    command.addArg("s/erver", "");
    command.addArg("p/ort", "");
    command.addArg("l/ogin", "");
    command.addArg("pass/word", "");
    command.addArg("ssl", "");
}

void registerEmailRecipientsCommand() {
    Command getCmd = cli.addCmd("email-recipients-get", [](cmd *commandPointer) {
        Command cmd(commandPointer);
        int amount = emailRecipientsGetAmount();
        if (amount) {
            resultMessageLen = STRING_LENGTH * (amount);
            resultMessage = (char *)malloc(resultMessageLen);
            char *resultMessagePtr = resultMessage;
            char recipient[STRING_LENGTH];
            for (int i = 0; i < amount; i++) {
                emailRecipientGetByIndex(i, recipient, STRING_LENGTH);
                resultMessagePtr += sprintf(resultMessagePtr, "%s%s", i ? "" : "\n", recipient);
            }
        } else {
            resultMessageLen = STRING_LENGTH;
            resultMessage = (char *)malloc(resultMessageLen);
            sprintf(resultMessage, "%No recipients found");
        }
    });
    Command postCmd = cli.addBoundlessCommand("email-recipients-post", [](cmd *commandPointer) {
        Command cmd(commandPointer);
        int argsCount = cmd.countArgs();
        if (argsCount) {
            for (int i = 0; i < argsCount; i++) {
                String recipient = cmd.getArg(i).getValue();
                emailRecipientAdd(recipient.c_str());
            }
        } else {
            resultMessageLen = STRING_LENGTH;
            resultMessage = (char *)malloc(resultMessageLen);
            sprintf(resultMessage, "%s At least one recipient needed", status_error_message);
        }
        
    });
    Command deleteCmd = cli.addBoundlessCommand("email-recipients-delete", [](cmd *commandPointer) {
        Command cmd(commandPointer);
        int argsCount = cmd.countArgs();
        if (argsCount) {
            for (int i = 0; i < argsCount; i++) {
                String recipient = cmd.getArg(i).getValue();
                emailRecipientDelete(recipient.c_str());
            }
        } else {
            emailRecipientsDeleteAll();
        }
    });
}

void registerModeCommand() {
    Command command = cli.addCmd(commands[MODE_COMMAND], [](cmd *commandPointer) {
        Command cmd(commandPointer);
        String state = cmd.getArg("state").getValue();
        state.toLowerCase();
        if (state.isEmpty()) {
            resultMessageLen = STRING_LENGTH;
            resultMessage = (char *)malloc(resultMessageLen);
            device_state_t state = getState();
            switch (state)
            {
            case DEVICE_STATE_CONFIG_BLUETOOTH:
                resultMessage = strcpy(resultMessage, "bt");
                break;
            case DEVICE_STATE_CONFIG_WIFI_HOTSPOT:
                resultMessage = strcpy(resultMessage, "hotspot");
                break;
            case DEVICE_STATE_WORK_WIFI_ALWAYS_CONNECTED:
                resultMessage = strcpy(resultMessage, "wifi");
                break;
            case DEVICE_STATE_WORK_WIFI_FOR_NOTIFICATION:
                resultMessage = strcpy(resultMessage, "work");
                break;
            default:
                break;
            }
        } else {
            if (state.equals("next")) {
                switchStateToNext();
            } else if (state.equals("bt")) {
                setState(DEVICE_STATE_CONFIG_BLUETOOTH);
            } else if (state.equals("hotspot")) {
                setState(DEVICE_STATE_CONFIG_WIFI_HOTSPOT);
            } else if (state.equals("wifi")) {
                setState(DEVICE_STATE_WORK_WIFI_ALWAYS_CONNECTED);
            } else if (state.equals("work")) {
                setState(DEVICE_STATE_WORK_WIFI_FOR_NOTIFICATION);
            } else {
                resultMessageLen = STRING_LENGTH;
                resultMessage = (char *)malloc(resultMessageLen);
                sprintf(resultMessage, "%s Wrong state", status_error_message);
            }
        }
    });
    command.addArg("state", "");
}

void registerRestartCommand() {
    Command command = cli.addCmd(commands[RESTART_COMMAND], [](cmd *commandPointer) {
        scheduleRestart(500);
    });
}
