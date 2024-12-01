#ifndef __WASTETEMPERATURETASK__
#define __WASTETEMPERATURETASK__

#include "kernel/Task.hpp"
#include "model/SWDSystem.hpp"

class WasteTemperatureTask : public Task {
   public:
    enum class State { VALID, HOT, PROBLEM, SOLVED };
    WasteTemperatureTask(SWDSystem* SWD_system);
    void init(int period);
    void tick();
    void setState(State s);

   private:
    SWDSystem* SWD_system;
    State state;
    float t;
    unsigned long hot_timer;
};

#endif
