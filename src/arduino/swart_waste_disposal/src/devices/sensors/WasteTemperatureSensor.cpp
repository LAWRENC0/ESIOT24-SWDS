#include "devices/sensors/WasteTemperatureSensor.hpp"
#include "config.hpp"
#include <Arduino.h>

bool WasteTemperatureSensor::init() {
    float temp;
    do {
        temp = detect();
    } while (temp > MAX_WASTE_TEMP);
    return true;
}

float WasteTemperatureSensor::detect() {
    int raw_value = analogRead(WTS_PIN);

    float voltage_MV = raw_value * 4.8876;
    float temperature_C = (voltage_MV - 500) / 10.0;
    return temperature_C;
}