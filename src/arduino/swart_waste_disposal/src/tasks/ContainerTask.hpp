#ifndef __CONTAINERTASK__
#define __CONTAINERTASK__

#include "kernel/Task.hpp"
#include "devices/buttons/Button.hpp"
#include "model/SWDSystem.hpp"

class ContainerTask : public Task {
   public:
    enum class State {
        CLOSED_ACCEPTING,
        OPEN_USER,
        CLOSED_WAIT,
        CLOSED_HIGH_TEMPERATURE,
        CLOSED_FULL,
        OPEN_OPERATOR,
        CLOSED_OPERATOR
    };

    ContainerTask(SWDSystem* SWD_system);
    void init(int period);
    void tick();
    void setState(State s);
    void setState(SWDSystem::CTState s);

   private:
    long stateTimeStamp;
    SWDSystem* SWD_system;
    State state;

    long elapsedTimeInState();
};

#endif
