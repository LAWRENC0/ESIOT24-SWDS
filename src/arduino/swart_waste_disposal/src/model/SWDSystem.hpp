#ifndef __SYSTEM__
#define __SYSTEM__

#include "kernel/Task.hpp"
#include "config.hpp"
#include "devices/sensors/UserDetectionSensor.hpp"
#include "devices/sensors/WasteLevelSensor.hpp"
#include "devices/sensors/WasteTemperatureSensor.hpp"
#include "devices/buttons/Button.hpp"
#include "devices/doors/Door.hpp"
#include "devices/lights/TwoLedMonitor.hpp"
#include "devices/consoles/UserConsole.hpp"

class SWDSystem {
   public:
    enum class CTState {
        CLOSED_ACCEPTING,
        OPEN_USER,
        CLOSED_WAIT,
        CLOSED_HIGH_TEMPERATURE,
        CLOSED_FULL,
        OPEN_OPERATOR,
        CLOSED_OPERATOR
    };
    enum class UDTState { WAKING_UP, AWAKE, ASLEEP };
    enum class WLTState { ACCEPT_WASTE, FULL, EMPTIED };
    enum class WTTState { VALID, HOT, PROBLEM, SOLVED };

    SWDSystem();
    void init();
    bool addTask(Task* task);
    void wakeUp();
    void sleep();

    bool checkEmptyDoneMsg();
    bool checkRestoreDoneMsg();

    SWDSystem::UDTState getUDTState();
    bool canSystemSleep();
    bool isSystemUnavailable();
    void setUDTState(SWDSystem::UDTState state);

    SWDSystem::CTState getCTState();
    void setCTState(SWDSystem::CTState state);

    SWDSystem::WLTState getWLTState();
    void setWLTState(SWDSystem::WLTState state);

    SWDSystem::WTTState getWTTState();
    void setWTTState(SWDSystem::WTTState state);

    bool isDoorTransitioning();
    bool isDoorOpenUser();

    bool isBOPressed();
    bool isBCPressed();

    bool detectUser();

    float detectWasteLevel();

    float detectWasteTemp();

   private:
    void updateUCState();

    const String empty_done_message = "empd";
    const String restore_done_message = "rstd";
    bool checkReceivedMsg(String s);
    const String start_message = "strt";
    const String system_prefix = "swd:";
    const String log_prefix = "lo:";
    const String msg_state_prefix = "st:";
    UserConsole* lcd;

    UDTState udt_state;
    CTState ct_state;
    WLTState wlt_state;
    WTTState wtt_state;

    int n_tasks;
    Task* task_list[MAX_TASKS];

    Button* b_open;
    Button* b_close;

    Door* door;
    bool door_transition_state;
    bool doorMovedTo(Door::State state);

    UserDetectionSensor* user_detection_sensor;

    WasteLevelSensor* waste_level_sensor;
    float waste_level;

    WasteTemperatureSensor* waste_temp_sensor;
    float temp;

    TwoLedMonitor* led_monitor;
};

#endif