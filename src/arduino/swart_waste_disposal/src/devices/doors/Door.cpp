#include "devices/doors/Door.hpp"
#include <Arduino.h>

Door::Door(ServoMotor* servo_motor) {
    this->servo_motor = servo_motor;
    this->setPowerState(true);
    this->curr_state = State::CLOSED;
    this->position = (int)curr_state;
}

Door::State Door::getCurrState() { return this->curr_state; }

void Door::setDestState(State dest_state) { this->dest_state = dest_state; }

bool Door::move() {
    if (curr_state == dest_state) {
        return false;
    } else {
        int delta = anglePerMove * abs((int)dest_state - (int)curr_state) / ((int)dest_state - (int)curr_state);
        position += delta;
        servo_motor->setPosition(position);
        if ((int)dest_state == position) curr_state = dest_state;
        return true;
    }
}

bool Door::getPowerState() { return this->power_state; }

void Door::setPowerState(bool new_state) {
    this->power_state = new_state;
    if (power_state) {
        this->servo_motor->on();
    } else {
        this->servo_motor->off();
    }
}
