#include "kernel/Scheduler.hpp"
#include "kernel/Task.hpp"
#include "model/SWDSystem.hpp"

#include "tasks/ContainerTask.hpp"
#include "tasks/UserDetectionTask.hpp"
#include "tasks/WasteLevelTask.hpp"
#include "tasks/WasteTemperatureTask.hpp"

#include "kernel/LoggerService.hpp"

#include <Arduino.h>

Scheduler sched;
SWDSystem SWD_system;

void setup() {
    Serial.begin(115200);

    SWD_system.init();

    ContainerTask* container_task = new ContainerTask(&SWD_system);
    container_task->init(CT_PERIOD_MS);

    WasteLevelTask* waste_level_task = new WasteLevelTask(&SWD_system);
    waste_level_task->init(WLT_PERIOD_MS);

    WasteTemperatureTask* waste_temperature_task = new WasteTemperatureTask(&SWD_system);
    waste_temperature_task->init(WTT_PERIOD_MS);

    UserDetectionTask* user_detection_task = new UserDetectionTask(&SWD_system);
    user_detection_task->init(UDT_PERIOD_MS);

    sched.init(SCHEDULER_PERIOD_MS);
    sched.addTask(user_detection_task);
    sched.addTask(waste_level_task);
    sched.addTask(waste_temperature_task);
    sched.addTask(container_task);

    SWD_system.addTask(user_detection_task);
    SWD_system.addTask(waste_level_task);
    SWD_system.addTask(waste_temperature_task);
    SWD_system.addTask(container_task);
    Logger.log(F("||    STARTING    ||"));
}

void loop() { sched.schedule(); }