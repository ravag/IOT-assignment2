#ifndef __TAKE_OFF_TASK__
#define __TAKE_OFF_TASK__

#include "kernel/Task.h"
#include "devices/Sonar.h"
#include "devices/ServoMotor.h"
#include "devices/TempSensor.h"
#include "model/Context.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include <LiquidCrystal_I2C.h>

#define CLOSING_DISTANCE 1.0
#define DISTANCE_TIME 3000
#define TIME_TO_OPEN 5000

class TakeOffTask : public Task {

public:
    TakeOffTask(Context* pContext, Sonar* pSensor, ServoMotor* pMotor, LiquidCrystal_I2C* pLcd, TempSensor* pTemp);
    void tick();

private:
    Context* context;
    Sonar* sensor;
    ServoMotor* motor;
    LiquidCrystal_I2C* lcd;
    TempSensor* temp;

    Msg* msg;

    float dist;
    long timePass;
    long lastTime;
    long dt;
    bool alreadyOver;
    bool alarm;
    bool justEntered;
    int timeInState;

    enum State {OPENING, CLOSING, ALARM, IDLE, OPEN} state;

    void setState(State s);
};

#endif