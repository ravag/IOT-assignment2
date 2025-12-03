#ifndef __LANDING_TASK__
#define __LANDING_TASK__

#include "devices\ServoMotor.h"
#include "devices\PresenceSensor.h"
#include "devices\ProximitySensor.h"
#include <LiquidCrystal_I2C>
#include "kernel\Task.h"
#include "model\Context.h"
#include <Arduino.h>

#define TIME2 3000

class LandingTask: public Task {
    public:
        LandingTask(ServoMotor* pMotor, PresenceSensor* pPIR, ProximitySensor* pSonar, LiquidCrystal_I2C* pLCD, Context* pContext);
        void tick();

    private:
        void setState(State state);
        void log(String& msg);

        bool checkAndSetJustEntered();

        enum State { IDLE, DOOR_OPENING, DOOR_OPEN, DRONE_LANDED, DOOR_CLOSING, ALARM } state;
        bool justEntered;

        ServoMotor* pMotor;
        PresenceSensor* pPIR;
        ProximitySensor* pSonar;
        LiquidCrystal_I2C* pLCD;
        Context* pContext;
        long lastTime;
}

#endif