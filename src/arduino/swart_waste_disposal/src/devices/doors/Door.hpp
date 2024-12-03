#ifndef __DOOR__
#define __DOOR_

#include "devices/motors/ServoMotor.hpp"

class Door {
   public:
    enum class State { OPEN_USER = 180, CLOSED = 90, OPEN_OPERATOR = 0 };
    const int anglePerMove = 10;
    Door(ServoMotor* servo_motor);
    State getCurrState();
    void setDestState(State new_state);
    bool move();
    bool getPowerState();
    void setPowerState(bool new_state);

   private:
    State curr_state;
    State dest_state;
    bool power_state;
    int position;
    ServoMotor* servo_motor;
};

#endif