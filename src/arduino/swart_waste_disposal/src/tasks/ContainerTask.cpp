#include "tasks/ContainerTask.hpp"
#include "Arduino.h"
#include "config.hpp"

ContainerTask::ContainerTask(SWDSystem* SWD_system) {
    this->can_sleep = true;
    this->SWD_system = SWD_system;
    this->setState(State::CLOSED_OPERATOR);
}

void ContainerTask::init(int period) { Task::init(period); }

void ContainerTask::tick() {
    bool justEntered = false;
    if (((int)SWD_system->getCTState()) != ((int)state)) {
        setState(SWD_system->getCTState());
        justEntered = true;
    }
    bool isTransitioning = SWD_system->isDoorTransitioning();
    switch (state) {
        case State::CLOSED_ACCEPTING:  // closed state waiting for user
            if ((SWD_system->isBOPressed() && !(SWD_system->isBCPressed())) || isTransitioning) {
                SWD_system->setCTState(SWDSystem::CTState::OPEN_USER);
            } else if (justEntered) {
                SWD_system->setCTState(SWDSystem::CTState::CLOSED_ACCEPTING);
            }
            break;
        case State::OPEN_USER:  // open state waiting for user
            if (((elapsedTimeInState() > WASTE_DISPOSAL_TIME) || SWD_system->isBCPressed()) || isTransitioning) {
                SWD_system->setCTState(SWDSystem::CTState::CLOSED_WAIT);
            }
            break;
        case State::CLOSED_WAIT:  // closed state
            if (isTransitioning) {
                SWD_system->setCTState(SWDSystem::CTState::CLOSED_WAIT);
            } else if (((elapsedTimeInState() > CLOSED_WAIT_TIME))) {
                SWD_system->setCTState(SWDSystem::CTState::CLOSED_ACCEPTING);
            }
            break;
        case State::CLOSED_FULL:  // closed state waiting for operator
            if (isTransitioning) SWD_system->setCTState(SWDSystem::CTState::CLOSED_FULL);
            break;
        case State::CLOSED_HIGH_TEMPERATURE:  // closed state waiting for operator
            if (isTransitioning) SWD_system->setCTState(SWDSystem::CTState::CLOSED_HIGH_TEMPERATURE);
            break;
        case State::OPEN_OPERATOR:  // open state waiting for operator
            SWD_system->setCTState(SWDSystem::CTState::OPEN_OPERATOR);
            break;
        case State::CLOSED_OPERATOR:  // open state waiting for operator
            SWD_system->setCTState(SWDSystem::CTState::CLOSED_OPERATOR);
            break;
        default:
            break;
    }
}

void ContainerTask::setState(State s) {
    state = s;
    stateTimeStamp = millis();
}
void ContainerTask::setState(SWDSystem::CTState s) {
    if (s == SWDSystem::CTState::CLOSED_ACCEPTING) return setState(State::CLOSED_ACCEPTING);
    if (s == SWDSystem::CTState::CLOSED_FULL) return setState(State::CLOSED_FULL);
    if (s == SWDSystem::CTState::CLOSED_HIGH_TEMPERATURE) return setState(State::CLOSED_HIGH_TEMPERATURE);
    if (s == SWDSystem::CTState::CLOSED_WAIT) return setState(State::CLOSED_WAIT);
    if (s == SWDSystem::CTState::OPEN_OPERATOR) return setState(State::OPEN_OPERATOR);
    if (s == SWDSystem::CTState::OPEN_USER) return setState(State::OPEN_USER);
    if (s == SWDSystem::CTState::CLOSED_OPERATOR) return setState(State::CLOSED_OPERATOR);
}

long ContainerTask::elapsedTimeInState() { return millis() - stateTimeStamp; }
