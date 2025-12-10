#include "BlinkingTask.h"
#include "config.h"
#include <Arduino.h>
#include "kernel\Logger.h"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext):
    pContext(pContext), pLed(pLed){
        setState(IDLE);
    }

void BlinkingTask::tick(){
    switch (state){
        case IDLE: {
            if(this->checkAndSetJustEntered()){
                pLed->switchOff();
                //Logger.log(F("[BT] IDLE"));
            }

            if(pContext->isInAlarm()){
                setState(OFF);
            }

            if(pContext->isBlinking()){
                setState(ON);
            }

            break;
        }

        case OFF: {
            if(this->checkAndSetJustEntered()){
                pLed->switchOff();
                //Logger.log(F("[BT] OFF"));
            }

            if(pContext->isBlinking()){
                setState(ON);
            } else if(!pContext->isInAlarm()) {
                setState(IDLE);
            }

            break;
        }

        case ON: {
            if(this->checkAndSetJustEntered()){
                pLed->switchOn();
                //Logger.log(F("[BT] ON"));
            }

            if(pContext->isBlinking()){
                setState(OFF);
            } else if(!pContext->isInAlarm()) {
                setState(IDLE);
            }

            break;
        }
    }
}

void BlinkingTask::setState(State s){
    state = s;
    justEntered = true; 
}

bool BlinkingTask::checkAndSetJustEntered(){
    bool bak = justEntered;

    if(justEntered){
        justEntered = false;
    }

    return bak;
}   