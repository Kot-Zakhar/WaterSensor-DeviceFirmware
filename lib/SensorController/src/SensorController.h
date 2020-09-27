#ifndef _SENSOR_CONTROLLER_H_
#define _SENSOR_CONTROLLER_H_

#include <Arduino.h>
#include <Ticker.h>
#include <IOController.h>

int getSensorValue();
void initSensorController();
int updateSensorValue();

#endif