#ifndef __CONFIG__
#define __CONFIG__

//device pins
#define MOTOR_PIN 13
#define TRIG_PIN 11
#define ECHO_PIN 12
#define PROX_PIN 10
#define BUTTON_PIN 3
#define ON_LED 7
#define IN_ACTION_LED 6
#define ALARM_LED 5
#define TEMP_PIN A0

//costanti per Temp
#define TEMP_T1 30
#define TEMP_T2 33
#define T_MAX_3 4000
#define T_MAX_4 3000

//costanti per Landing e TakeOff
#define DISTANCE_TIME 3000
#define TIME_TO_OPEN 5000.0

#endif