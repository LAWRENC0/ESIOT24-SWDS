#include "tasks/WasteTemperatureTask.hpp"
#include "config.hpp"
#include "Arduino.h"

WasteTemperatureTask::WasteTemperatureTask(SWDSystem* SWD_system) {
    this->can_sleep = true;
    this->SWD_system = SWD_system;
    this->setState(State::VALID);
}

void WasteTemperatureTask::init(int period) { Task::init(period); }

void WasteTemperatureTask::tick() {
    switch (state) {
        case State::VALID:
            t = SWD_system->detectWasteTemp();
            if (t >= MAX_WASTE_TEMP) {
                hot_timer = millis();
                SWD_system->setWTTState(SWDSystem::WTTState::HOT);
                setState(State::HOT);
            }
            break;
        case State::HOT:
            t = SWD_system->detectWasteTemp();
            if (t < MAX_WASTE_TEMP) {
                SWD_system->setWTTState(SWDSystem::WTTState::VALID);
                setState(State::VALID);
            } else if (((millis() - hot_timer) >= MAX_HOT_TIME) && !SWD_system->isSystemUnavailable()) {
                SWD_system->setWTTState(SWDSystem::WTTState::PROBLEM);
                setState(State::PROBLEM);
            }
            break;
        case State::PROBLEM:
            if (SWD_system->checkRestoreDoneMsg()) {
                SWD_system->setWTTState(SWDSystem::WTTState::SOLVED);
                setState(State::SOLVED);
            }
            break;
        case State::SOLVED:
            SWD_system->setWTTState(SWDSystem::WTTState::VALID);
            setState(State::VALID);
            break;
        default:
            break;
    }
}

void WasteTemperatureTask::setState(State s) { state = s; }