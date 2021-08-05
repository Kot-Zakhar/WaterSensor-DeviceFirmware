#include "SensorsService.h"
#include "DHTController.h"
#include "WaterSensorController.h"

int getCurrentSensorsValues(SensorsValues &values) {
    values.water = getWaterSensorValue();
    bool hum = getHumidity(&values.humidity);
    bool temp = getTemperature(&values.temperature);
    return hum | temp | (values.humidity < 0.1) | (values.temperature < 0.1);
}