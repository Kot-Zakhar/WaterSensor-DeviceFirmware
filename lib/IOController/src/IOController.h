#pragma once

#ifndef _IO_CONTROLLER_H_
#define _IO_CONTROLLER_H_

#define IO_WRITE_NOP 0
#define IO_WRITE_SERIAL 2
#define IO_WRITE_SCREEN 4
#define IO_WRITE_CLEAN_BEFORE_WRITE 8

#include <Adafruit_SSD1306.h>

void InitIOController();
void ClearDisplay();
void IOWrite(int writeTo, const char *str);

#endif