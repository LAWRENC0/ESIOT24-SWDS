#include "devices/sensors/WasteLevelSensor.hpp"
#include "config.hpp"
#include <Arduino.h>

bool WasteLevelSensor::init() {
    pinMode(WLS_TRIG_PIN, OUTPUT);
    pinMode(WLS_ECHO_PIN, INPUT);
    float d;
    do {
        d = detect();
        delay(1000);

    } while (d < MIN_WASTE_DISTANCE);
    return true;
}

float WasteLevelSensor::detect() {
    digitalWrite(WLS_TRIG_PIN, LOW);
    delayMicroseconds(3);
    digitalWrite(WLS_TRIG_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(WLS_TRIG_PIN, LOW);

    float tUS = pulseIn(WLS_ECHO_PIN, HIGH);
    if (tUS == 0) {
        return 1000000.0;
    }
    float t = tUS / 1000.0 / 1000.0 / 2;
    float d = t * vs;
    return d;
}