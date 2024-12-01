#include "devices/sensors/UserDetectionSensor.hpp"
#include "config.hpp"
#include <Arduino.h>

bool UserDetectionSensor::init() {
    unsigned long ts = millis();
    for (; millis() - ts < UDS_CALIBRATION_TIME_MS;);
    return true;
}

bool UserDetectionSensor::detect() { return digitalRead(UDS_PIN); }