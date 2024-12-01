#include "devices/motors/ServoMotorImpl.hpp"
#include "devices/buttons/ButtonImpl.hpp"
#include "config.hpp"
#include "model/SWDSystem.hpp"
#include "kernel/MsgService.hpp"
#include "kernel/LoggerService.hpp"

SWDSystem::SWDSystem() {}

void SWDSystem::init() {
    MsgService.init();
    bool start = false;
    while (start == false) {
        start = MsgService.checkStartMessage(start_message);
    }
    Logger.log(F("|| STARTING: TEST ||"));

    n_tasks = 0;
    // USER CONSOLE
    Logger.log(F("UserConsole..."));
    lcd = new UserConsole();
    lcd->init();
    lcd->turnOnDisplay();
    lcd->test();
    delay(1000);
    Logger.log(F("---Done: valid state."));
    // BUTTONS
    b_open = new ButtonImpl(B_OPEN_PIN);
    b_open->sync();
    b_close = new ButtonImpl(B_CLOSE_PIN);
    b_close->sync();
    // DOOR
    door = new Door(new ServoMotorImpl(SERVO_PIN));
    Logger.log(F("Door..."));
    door->setDestState(Door::State::OPEN_USER);
    while (door->move()) {
        delay(40);
    }
    delay(1000);
    door->setDestState(Door::State::CLOSED);
    while (door->move()) {
        delay(40);
    }
    Logger.log(F("---Done: valid state."));
    door_transition_state = false;
    ct_state = CTState::CLOSED_ACCEPTING;
    // USER DETECTION
    user_detection_sensor = new UserDetectionSensor();
    Logger.log(F("UDSensor..."));
    if (!user_detection_sensor->init()) abort();
    Logger.log(F("---Done: valid state."));
    udt_state = UDTState::AWAKE;
    // WASTE LEVEL
    waste_level_sensor = new WasteLevelSensor();
    Logger.log(F("WLSensor..."));
    if (!waste_level_sensor->init()) abort();
    Logger.log(F("---Done: valid state."));
    wlt_state = WLTState::ACCEPT_WASTE;
    // WASTE TEMPERATURE
    waste_temp_sensor = new WasteTemperatureSensor();
    Logger.log(F("WTSensor..."));
    if (!waste_temp_sensor->init()) abort();
    Logger.log(F("---Done: valid state."));
    wtt_state = WTTState::VALID;
    // LEDS
    led_monitor = new TwoLedMonitor(new Led(GREEN_LED_PIN), new Led(RED_LED_PIN));
}

bool SWDSystem::addTask(Task* task) {
    if (n_tasks < MAX_TASKS - 1) {
        task_list[n_tasks] = task;
        n_tasks++;
        return true;
    } else {
        return false;
    }
}

void SWDSystem::sleep() {
    for (Task* task : task_list) {
        if (task->canSleep()) task->setActive(false);
    }
    lcd->prepareToSleep();
}

void SWDSystem::wakeUp() {
    for (Task* task : task_list) {
        task->setActive(true);
    }
    lcd->resumeFromSleeping();
}

SWDSystem::UDTState SWDSystem::getUDTState() { return udt_state; }
SWDSystem::WLTState SWDSystem::getWLTState() { return wlt_state; }
SWDSystem::WTTState SWDSystem::getWTTState() { return wtt_state; }
SWDSystem::CTState SWDSystem::getCTState() { return ct_state; }

void SWDSystem::setUDTState(UDTState new_state) {
    switch (new_state) {
        case UDTState::WAKING_UP:
            this->wakeUp();
        case UDTState::AWAKE:
            break;
        case UDTState::ASLEEP:
            this->sleep();
            break;
        default:
            break;
    }
    udt_state = new_state;
}

void SWDSystem::setWLTState(WLTState new_state) {
    switch (new_state) {
        case WLTState::ACCEPT_WASTE:
            break;
        case WLTState::FULL:
            lcd->display(UserConsole::Messages::CONTAINER_FULL);
            led_monitor->setState(TwoLedMonitor::State::UNAVAILABLE);
            setCTState(CTState::CLOSED_FULL);
            break;
        case WLTState::EMPTIED:
            setCTState(CTState::OPEN_OPERATOR);
            break;
        default:
            break;
    }
    wlt_state = new_state;
}

void SWDSystem::setWTTState(WTTState new_state) {
    switch (new_state) {
        case WTTState::VALID:
            break;
        case WTTState::HOT:
            break;
        case WTTState::PROBLEM:
            lcd->display(UserConsole::Messages::PROBLEM);
            led_monitor->setState(TwoLedMonitor::State::UNAVAILABLE);
            setCTState(CTState::CLOSED_HIGH_TEMPERATURE);
            break;
        case WTTState::SOLVED:
            setCTState(CTState::OPEN_OPERATOR);
            break;
        default:
            break;
    }
    wtt_state = new_state;
}

void SWDSystem::setCTState(CTState new_state) {
    switch (new_state) {
        case CTState::CLOSED_ACCEPTING:
            if (doorMovedTo(Door::State::CLOSED)) {
                ct_state = new_state;
                led_monitor->setState(TwoLedMonitor::State::AVAILABLE);
                lcd->display(UserConsole::Messages::ENTER_WASTE);
            }
            break;
        case CTState::OPEN_USER:
            if (doorMovedTo(Door::State::OPEN_USER)) {
                ct_state = new_state;
                lcd->display(UserConsole::Messages::PRESS_CLOSE);
            }
            break;
        case CTState::CLOSED_WAIT:
            ct_state = new_state;
            if (doorMovedTo(Door::State::CLOSED)) {
                lcd->display(UserConsole::Messages::WASTE_RECEIVED);
            }
            break;
        case CTState::CLOSED_HIGH_TEMPERATURE:
            ct_state = CTState::CLOSED_HIGH_TEMPERATURE;
            if (doorMovedTo(Door::State::CLOSED)) {
            }
            break;
        case CTState::CLOSED_FULL:
            ct_state = CTState::CLOSED_FULL;
            if (doorMovedTo(Door::State::CLOSED)) {
            }
            break;
        case CTState::OPEN_OPERATOR:
            if (doorMovedTo(Door::State::OPEN_OPERATOR)) {
                ct_state = CTState::CLOSED_OPERATOR;
            } else {
                ct_state = CTState::OPEN_OPERATOR;
            }
            break;
        case CTState::CLOSED_OPERATOR:
            if (doorMovedTo(Door::State::CLOSED)) {
                ct_state = CTState::CLOSED_ACCEPTING;
            } else {
                ct_state = CTState::CLOSED_OPERATOR;
            }
            break;
        default:
            break;
    }
}

bool SWDSystem::checkEmptyDoneMsg() { return checkReceivedMsg(empty_done_message); }

bool SWDSystem::checkRestoreDoneMsg() { return checkReceivedMsg(restore_done_message); }

bool SWDSystem::checkReceivedMsg(String s) {
    bool done = false;
    String content;
    if (MsgService.isMsgAvailable()) {
        Msg* msg = MsgService.receiveMsg();
        if (msg != NULL) {
            content = msg->getContent();
            content.trim();
            if (content == s) {
                done = true;
            }
            delete msg;
        }
    }
    return done;
}

bool SWDSystem::canSystemSleep() {
    return (ct_state == CTState::CLOSED_ACCEPTING) && (wtt_state == WTTState::VALID) &&
           (wlt_state == WLTState::ACCEPT_WASTE);
}

bool SWDSystem::isSystemUnavailable() {
    bool pred = (wtt_state == WTTState::PROBLEM) || (wlt_state == WLTState::FULL);
    return pred;
}

bool SWDSystem::isDoorTransitioning() { return door_transition_state; }

bool SWDSystem::isDoorOpenUser() { return ct_state == CTState::OPEN_USER; }

bool SWDSystem::doorMovedTo(Door::State state) {
    door->setDestState(state);
    bool isDoorReady = !(door->move());
    if (!isDoorReady) {
        door_transition_state = true;
        return false;
    } else {
        door_transition_state = false;
        return true;
    }
}

bool SWDSystem::isBOPressed() {
    this->b_open->sync();
    return this->b_open->isPressed();
}
bool SWDSystem::isBCPressed() {
    this->b_close->sync();
    return this->b_close->isPressed();
}

bool SWDSystem::detectUser() {
    bool detected = this->user_detection_sensor->detect();
    updateUCState();
    return detected;
}

float SWDSystem::detectWasteLevel() {
    waste_level = this->waste_level_sensor->detect();
    updateUCState();
    return waste_level;
}

float SWDSystem::detectWasteTemp() {
    temp = waste_temp_sensor->detect();
    updateUCState();
    return temp;
}

void SWDSystem::updateUCState() {
    MsgService.sendMsg(system_prefix + msg_state_prefix + String((int)ct_state) + ":" +
                       String(waste_level).substring(0, 5) + ":" + String(temp).substring(0, 5) + ":" +
                       String((int)udt_state));
}