#include <SensorController.h>

#define SENSOR_PIN 36
int16_t value = 0;

void initSensorController(){
    pinMode(SENSOR_PIN, INPUT);
    updateSensorValue();
}

int updateSensorValue(){
    value = analogRead(SENSOR_PIN);
    return value;
}

int getSensorValue(){
    return value;
}
