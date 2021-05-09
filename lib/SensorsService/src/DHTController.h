#ifndef _DHT_CONTROLLER_H_
#define _DHT_CONTROLLER_H_

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

void initDHTController();
int getTemperature(float *value);
int getHumidity(float *value);


#endif