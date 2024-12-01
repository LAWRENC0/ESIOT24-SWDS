#include "tasks/WasteLevelTask.hpp"
#include "Arduino.h"
#include "config.hpp"

WasteLevelTask::WasteLevelTask(SWDSystem* SWD_system) {
    this->can_sleep = true;
    this->SWD_system = SWD_system;
    this->setState(State::ACCEPT_WASTE);
}

void WasteLevelTask::init(int period) { Task::init(period); }

void WasteLevelTask::tick() {
    switch (state) {
        case State::ACCEPT_WASTE:
            distance_to_waste = SWD_system->detectWasteLevel();
            if ((SWD_system->isDoorOpenUser()) && (distance_to_waste < MIN_WASTE_DISTANCE) &&
                (!SWD_system->isSystemUnavailable())) {
                setState(State::FULL);
                SWD_system->setWLTState(SWDSystem::WLTState::FULL);
            }
            break;
        case State::FULL:
            if (SWD_system->checkEmptyDoneMsg()) {
                setState(State::EMPTIED);
                SWD_system->setWLTState(SWDSystem::WLTState::EMPTIED);
            }
            break;
        case State::EMPTIED:
            setState(State::ACCEPT_WASTE);
            SWD_system->setWLTState(SWDSystem::WLTState::ACCEPT_WASTE);
            break;
        default:
            break;
    }
}

void WasteLevelTask::setState(State s) { state = s; }
