#include "Task\BlinkingTask.h"
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
                Logger.log(F("[BT] IDLE"));
            }

            if(pContext->isInAlarm){
                setState(OFF);
            }

            if(this->isActive){
                setState(ON);
            }

            break;
        }

        case OFF: {
            if(this->checkAndSetJustEntered()){
                pLed->switchOff();
                Logger.log(F("[BT] IDLE"));
            }

            if(pContext->isInAlarm){
                setState(IDLE);
            } else {
                if(this->!isActive){
                    setState(IDLE);
                }
                setState(ON);
            }

            break;
        }

        case ON: {
            if(this->checkAndSetJustEntered()){
                pLed->switchOn();
                Logger.log(F("[BT] IDLE"));
            }

            if(pContext->isInAlarm){
                setState(IDLE);
            } else {
                if(this->!isActive){
                    setState(IDLE);
                }
                setState(OFF);
            }

            break;
        }
    }
}

void BlinkingTask::setState(int s){
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