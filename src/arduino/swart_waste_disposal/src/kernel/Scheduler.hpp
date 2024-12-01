#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "kernel/Task.hpp"
#include "config.hpp"

class Scheduler {
    long base_period;
    int n_tasks;
    Task* task_list[MAX_TASKS];

   public:
    void init(long base_period);
    virtual bool addTask(Task* task);
    virtual void schedule();
};

#endif
