#include <WaterSensorController.h>

#define SENSOR_PIN 36

void initWaterSensorController(){
    pinMode(SENSOR_PIN, INPUT);
}

int getWaterSensorValue(){
    return analogRead(SENSOR_PIN);
}
