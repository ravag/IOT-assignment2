#ifndef __TEMPTASK__
#define __TEMPTASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Led.h"
#include "devices/TempSensor.h"
#include <Arduino.h>

class TempTask: public Task {

    public:
        TempTask(Led* pLed, TempSensor* pTempSensor, Context* pContext);
        void tick();

    private:
        void setState(int state);
        long elapsedTimeInState();
        void log(const String& msg);

        bool checkAndSetJustEntered();

        enum { D_OUT, D_IN, O_T1, O_T2, W_NM, W_RS} state;
        long stateTimestamp;
        bool justEntered;

        Led* pLed;
        TempSensor* pTempSensor;
        Context* pContext;
};

#endif