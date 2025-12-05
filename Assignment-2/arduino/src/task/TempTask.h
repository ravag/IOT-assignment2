#ifndef __TEMPTASK__
#define __TEMPTASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Led.h"
#include "devices/TempSensorTMP36.h"
#include "devices/ButtonImpl.h"
#include "LiquidCrystal_I2C.h"
#include <Arduino.h>

class TempTask: public Task {

    public:
        TempTask(Led* pLed, TempSensor* pTempSensor, ButtonImpl* pButton, LiquidCrystal_I2C* plcd,Context* pContext);
        void tick();

    private:
        long elapsedTimeInState();
        void log(const String& msg);

        bool checkAndSetJustEntered();

        enum State { IDLE, O_T1, O_T2, W_NM, W_RS} state;
        long stateTimestamp;
        bool justEntered;

        void setState(State state);

        Led* pLed;
        TempSensor* pTempSensor;
        ButtonImpl* pButton;
        LiquidCrystal_I2C* plcd;
        Context* pContext;
};

#endif