#ifndef __WASTELEVELTASK__
#define __WASTELEVELTASK__

#include "kernel/Task.hpp"
#include "model/SWDSystem.hpp"

class WasteLevelTask : public Task {
   public:
    enum class State { ACCEPT_WASTE, FULL, EMPTIED };
    WasteLevelTask(SWDSystem* SWD_system);
    void init(int period);
    void tick();
    void setState(State s);

   private:
    SWDSystem* SWD_system;
    State state;
    float distance_to_waste;
};

#endif
