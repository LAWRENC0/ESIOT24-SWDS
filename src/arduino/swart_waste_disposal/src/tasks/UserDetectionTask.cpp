#include "tasks/UserDetectionTask.hpp"
#include "Arduino.h"
#include "config.hpp"

UserDetectionTask::UserDetectionTask(SWDSystem* SWD_system) {
    this->can_sleep = false;
    this->SWD_system = SWD_system;
    this->setState(State::AWAKE);
    this->last_detection_time = millis();
}

void UserDetectionTask::init(int period) { Task::init(period); }

void UserDetectionTask::tick() {
    switch (state) {
        case State::AWAKE:
            detected = SWD_system->detectUser();
            if (detected) {
                last_detection_time = millis();
            } else {
                if ((millis() - last_detection_time >= MAX_DETECTION_TIME)) {
                    setState(State::ASLEEP);
                    SWD_system->setUDTState(SWDSystem::UDTState::ASLEEP);
                }
            }
            break;
        case State::ASLEEP:
            detected = SWD_system->detectUser();
            if (detected) {
                last_detection_time = millis();
                setState(State::WAKING_UP);
                SWD_system->setUDTState(SWDSystem::UDTState::WAKING_UP);
            }
            break;
        case State::WAKING_UP:
            setState(State::AWAKE);
            SWD_system->setUDTState(SWDSystem::UDTState::AWAKE);
            break;
        default:
            break;
    }
}

void UserDetectionTask::setState(State s) { state = s; }
