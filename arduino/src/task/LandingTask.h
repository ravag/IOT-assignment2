#ifndef __LANDING_TASK__
#define __LANDING_TASK__

#include "devices\ServoMotor.h"
#include "devices\PresenceSensor.h"
#include "devices/Pir.h"
#include "devices\ProximitySensor.h"
#include "LiquidCrystal_I2C.h"
#include "kernel\Task.h"
#include "model\Context.h"
#include "kernel/MsgService.h"
#include <Arduino.h>

#define CLOSING_DISTANCE 1.0
#define DISTANCE_TIME 3000
#define TIME_TO_OPEN 5000

class LandingTask: public Task {
    public:
        LandingTask(ServoMotor* pMotor, Pir* pPIR, ProximitySensor* pSonar, LiquidCrystal_I2C* pLCD, Context* pContext);
        void tick();

    private:
        void log(String& msg);

        bool checkAndSetJustEntered();

        void closeDoor();
        void openDoor();
        bool isDoorOpen();
        bool isDroneNear();

        enum State { IDLE, DOOR_OPENING, DOOR_OPEN, DRONE_LANDED, DOOR_CLOSING, ALARM } state;
        bool justEntered;

        void setState(State state);

        ServoMotor* pMotor;
        Pir* pPIR;
        ProximitySensor* pSonar;
        LiquidCrystal_I2C* pLCD;
        Context* pContext;
        Msg* msg;
        long lastTime;
        long timeInState;
};

#endif