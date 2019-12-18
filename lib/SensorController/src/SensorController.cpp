#include <SensorController.h>

#define SENSOR_PIN 39
int16_t value = 0;

void InitSensorController(){
    pinMode(SENSOR_PIN, INPUT);
    UpdateSensorValue();
}

int UpdateSensorValue(){
    value = analogRead(SENSOR_PIN);
    return value;
}

int GetSensorValue(){
    return value;
}
