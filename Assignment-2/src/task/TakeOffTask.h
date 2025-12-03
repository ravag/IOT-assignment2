#ifndef __TAKE_OFF_TASK__
#define __TAKE_OFF_TASK__

#include "kernel/Task.h"
#include "devices/Sonar.h"
#include "devices/ServoMotor.h"
#include "devices/TempSensor.h"
#include "model/Context.h"
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

#define CLOSING_DISTANCE 1.0
#define DISTANCE_TIME 3000

class TakeOffTask : public Task {
private:
    LiquidCrystal_I2C* lcd;
    Sonar* sensor;
    ServoMotor* motor;
    Context* context;
    TempSensor* temp;

    long timePass;
    long lastTime;
    bool alreadyOver;

    enum State {OPENING, CLOSING, ALARM, IDLE, OPEN} state;

    void setState(State s);

public:
    TakeOffTask(Context* pContext, Sonar* pSensor, ServoMotor* pMotor, LiquidCrystal_I2C* pLcd, TempSensor* pTemp);
    void tick();
};

#endif