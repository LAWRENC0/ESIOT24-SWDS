#ifndef __USERDETECTIONTASK__
#define __USERDETECTIONTASK__

#include "kernel/Task.hpp"
#include "model/SWDSystem.hpp"

class UserDetectionTask : public Task {
   public:
    enum class State { AWAKE, ASLEEP, WAKING_UP };
    UserDetectionTask(SWDSystem* SWD_system);
    void init(int period);
    void tick();
    void setState(State s);

   private:
    SWDSystem* SWD_system;
    State state;
    bool detected;
    unsigned long last_detection_time;
};

#endif
