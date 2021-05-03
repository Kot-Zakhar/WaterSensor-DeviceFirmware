#ifndef _SENSOR_CHECKER_H_
#define _SENSOR_CHECKER_H_

#include <EmailController.h>
#include <SensorController.h>
#include <DHTController.h>

void initSensorChecker(bool isConfigState);
void processSensorChecker();
bool shouldSensorBeProcessed();
void stopSensorChecker();
void restartSensorChecker();

#endif