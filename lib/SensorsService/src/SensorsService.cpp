#include "SensorsService.h"
#include "DHTController.h"
#include "WaterSensorController.h"

void getCurrentSensorsValues(SensorsValues &values) {
    values.water = getWaterSensorValue();
    getHumidity(&values.humidity);
    getTemperature(&values.temperature);
}