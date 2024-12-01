#ifndef __TASK__
#define __TASK__

class Task {
    long my_period;
    long time_elapsed;
    bool active;

   protected:
    bool can_sleep;

   public:
    virtual void init(long period) {
        my_period = period;
        time_elapsed = 0;
        active = true;
    }

    virtual void tick() = 0;

    bool updateAndCheckTime(long base_period) {
        time_elapsed += base_period;
        if (time_elapsed >= my_period) {
            time_elapsed = 0;
            return true;
        } else {
            return false;
        }
    }

    bool isActive() { return active; }

    void setActive(bool active) { this->active = active; }

    bool canSleep() { return can_sleep; }
};

#endif
