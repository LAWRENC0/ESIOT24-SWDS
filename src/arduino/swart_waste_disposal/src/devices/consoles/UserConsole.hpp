#ifndef __USER_CONSOLE__
#define __USER_CONSOLE__

#include "config.hpp"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// #include "Bridge.h"

class UserConsole {
   public:
    enum class Messages { ENTER_WASTE, PRESS_CLOSE, WASTE_RECEIVED, CONTAINER_FULL, PROBLEM, WELCOME };

    UserConsole();

    void init();

    void turnOffDisplay();
    void turnOnDisplay();

    void display(Messages message);

    void prepareToSleep();
    void resumeFromSleeping();

    void test();

   private:
    LiquidCrystal_I2C* lcd;
};

#endif