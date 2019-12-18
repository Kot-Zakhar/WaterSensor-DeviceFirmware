#pragma once

#ifndef _IO_CONTROLLER_H_
#define _IO_CONTROLLER_H_

#include <Adafruit_SSD1306.h>
#include <Ticker.h>

#define MAX_LINES_AMOUNT 8

#define IO_WRITE_NOP 0
#define IO_WRITE_SERIAL 2
#define IO_WRITE_SCREEN 4
#define IO_WRITE_CLEAN_BEFORE_WRITE 8

enum message_t {
    BT_CONNECTED,
    BT_DISCONNECTED,

    BT_GOT_GOOD_COMMAND,
    BT_GOT_BAD_COMMAND,
    BT_PENDING_COMMAND,
    BT_END_COMMAND,

    WIFI_CONNECTED,
    WIFI_DISCONNECTED,

    MODE_CONFIG_ON,
    MODE_WORK_ON,

    Interrupt0,
    Interrupt1,
    Interrupt2,
    Interrupt3,

    SensorValueCnaged,
    SensorValueOutOfRange,

    WAIT,
    SUCCESS,
    WARNING,
    ERROR,
    FATAL_ERROR,
};

enum blink_duration_t {
    FAST_BLINK,
    BREATH,
};

enum rgb_color_t {
    RGB_WHITE,
    RGB_YELLOW,
    RGB_ORANGE,
    RGB_RED,
    RGB_ROSE,
    RGB_MAGNETTA,
    RGB_PURPLE,
    RGB_BLUE,
    RGB_SKY_BLUE,
    RGB_CYAN,
    RGB_OCEAN_GREEN,
    RGB_GREEN,
    RGB_LEAF_GREEN
};

void InitIOController();
void ClearDisplay();
void IOWrite(int writeTo, const char *str);
void IOIndicate(message_t messageType);

#endif