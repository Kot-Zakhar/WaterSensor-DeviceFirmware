#ifndef _SCREEN_CONTROLLER_H_
#define _SCREEN_CONTROLLER_H_

#include <Adafruit_SSD1306.h>
#include <custom_types.h>

void initScreen();
void updateScreen(device_state_t currentState);
void setPage(screen_page_t page);
screen_page_t getPage();

#endif