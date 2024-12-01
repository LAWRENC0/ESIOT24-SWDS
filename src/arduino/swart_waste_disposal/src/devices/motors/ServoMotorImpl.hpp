#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__

#include "devices/motors/ServoMotor.hpp"
#include <arduino.h>
#include "ServoTimer2.h"

class ServoMotorImpl : public ServoMotor {
   public:
    ServoMotorImpl(int pin);

    void on();
    void setPosition(int angle);
    int getAngle();
    void off();

   private:
    int pin;
    int angle;
    ServoTimer2 motor;
};

#endif
