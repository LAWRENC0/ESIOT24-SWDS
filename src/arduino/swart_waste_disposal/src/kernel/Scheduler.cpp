#include "kernel/Scheduler.hpp"
#include <TimerOne.h>
#include "kernel/LoggerService.hpp"

volatile bool timer_flag;

void timerHandler(void) { timer_flag = true; }

void Scheduler::init(long base_period) {
    this->base_period = base_period;
    timer_flag = false;
    long period = 1000l * base_period;
    Timer1.initialize(period);
    Timer1.attachInterrupt(timerHandler);
    n_tasks = 0;
}

bool Scheduler::addTask(Task* task) {
    if (n_tasks < MAX_TASKS - 1) {
        task_list[n_tasks] = task;
        n_tasks++;
        return true;
    } else {
        return false;
    }
}

void Scheduler::schedule() {
    while (!timer_flag) {
    }
    timer_flag = false;

    for (int i = 0; i < n_tasks; i++) {
        if (task_list[i]->updateAndCheckTime(base_period) && task_list[i]->isActive()) {
            task_list[i]->tick();
        }
    }
}
