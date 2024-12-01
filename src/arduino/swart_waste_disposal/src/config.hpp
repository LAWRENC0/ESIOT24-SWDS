#ifndef __CONFIG__
#define __CONFIG__

// SYSTEM AND SCHEDULER
#define MAX_TASKS 50
#define SCHEDULER_PERIOD_MS ((long)50)

// TEMPERATURE TASK: WCET 5ms
#define WTT_PERIOD_MS ((long)500)
#define MAX_WASTE_TEMP ((float)24)
#define MAX_HOT_TIME 3e3
#define WTS_PIN A3

// WASTE LEVEL TASK: WCET 5ms
#define WLT_PERIOD_MS ((long)250)
#define MIN_WASTE_DISTANCE (float)0.10
#define WLS_ECHO_PIN 7
#define WLS_TRIG_PIN 8

// USER DETECTION TASK: WCET 5ms
#define UDT_PERIOD_MS ((long)500)
#define MAX_DETECTION_TIME 1.5e4
#define UDS_CALIBRATION_TIME_MS 10e3
#define UDS_PIN 4

// CONTAINER TASK: WCET25ms = 20ms(door opening) + 5ms
#define CT_PERIOD_MS ((long)50)  // we need button input
#define WASTE_DISPOSAL_TIME 1e4
#define CLOSED_WAIT_TIME 5e3
#define SERVO_PIN 3

// BUTTONS
#define B_OPEN_PIN 10
#define B_CLOSE_PIN 11

// LEDS
#define GREEN_LED_PIN 12
#define RED_LED_PIN 13

#endif