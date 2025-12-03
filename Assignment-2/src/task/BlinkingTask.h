#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "devices\Led.h"
#include "kernel\Task.h"
#include "model\Context.h"
#include <Arduino.h>

class BlinkingTask: public Task {

    public:
        BlinkingTask(Led* pLed, Context* pContext);
        void tick();

    private:
        void setState(int State);
        void log(String& msg);

        bool checkAndSetJustEntered();

        enum { IDLE, OFF, ON } state;
        bool justEntered;

        Led* pLed;
        Context* pContext;
}

#endif