
#include "devices/consoles/UserConsole.hpp"
#include <Arduino.h>
#include <avr/sleep.h>
#include "config.hpp"

UserConsole::UserConsole() { lcd = new LiquidCrystal_I2C(0x27, 20, 4); }

void UserConsole::init() {
    lcd->init();
    lcd->backlight();
    lcd->noDisplay();
}

void UserConsole::turnOnDisplay() {
    lcd->display();
    lcd->clear();
}

void UserConsole::turnOffDisplay() { lcd->noDisplay(); }

void UserConsole::display(Messages message) {
    lcd->clear();
    lcd->setCursor(0, 0);
    switch (message) {
        case Messages::ENTER_WASTE:
            lcd->print(F("Press Open To Enter Waste"));
            break;
        case Messages::PRESS_CLOSE:
            lcd->print(F("Press Close When Done"));
            break;
        case Messages::WASTE_RECEIVED:
            lcd->print(F("Waste Received"));
            break;
        case Messages::CONTAINER_FULL:
            lcd->print(F("Container Full"));
            break;
        case Messages::PROBLEM:
            lcd->print(F("Problem Detected"));
            break;
        case Messages::WELCOME:
            lcd->print(F("Welcome"));
            break;

        default:
            break;
    }
}

void UserConsole::prepareToSleep() {
    lcd->noDisplay();
    lcd->noBacklight();
}

void UserConsole::resumeFromSleeping() {
    lcd->display();
    lcd->backlight();
}

void UserConsole::test() {
    delay(1000);

    lcd->display();
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(F("Testing"));
    delay(1000);
    lcd->clear();
}
