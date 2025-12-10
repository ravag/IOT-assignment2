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
    bool alreadyOver;
    bool alarm;
    bool justEntered;
    long timeInState;
    float currentPosition;

    enum State {OPENING, CLOSING, ALARM, IDLE, OPEN} state;

    void setState(State s);
    bool checkAndSetJustEntered();
    void openDoor();
    void closeDoor();
};

#endif