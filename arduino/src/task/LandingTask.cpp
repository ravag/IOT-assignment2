#include "LandingTask.h"
#include <Arduino.h>
#include "kernel\Logger.h"
#include "kernel/MsgService.h"

LandingTask::LandingTask(ServoMotor* pMotor, Pir* pPIR, ProximitySensor* pSonar, LiquidCrystal_I2C* pLCD, Context* pContext):
pMotor(pMotor), pPIR(pPIR), pSonar(pSonar), pLCD(pLCD), pContext(pContext) {
    setState(IDLE);
}

void LandingTask::tick(){
    if (pContext->isDroneIn()) {
        switch(state){
            case IDLE: {
                if(this->checkAndSetJustEntered()){
                    pLCD->clear();
                    pLCD->print("DRONE IN");
                    Logger.log(F("[LT] IDLE"));
                }

                if(pContext->isInAlarm()) {
                    setState(DOOR_CLOSING);
                }
                msg = MsgService.receiveMsg(); 
                if(!pContext->isInPreAlarm() && msg->getContent() == "rl" && this->isDroneNear()) {
                    setState(DOOR_OPENING);
                }

                break;
            }

            case DOOR_OPENING: {
                if(this->checkAndSetJustEntered()){     
                    pContext->setBlinkingOn();
                    pLCD->clear();
                    pLCD->print("LANDING");
                    pMotor->on();
                    Logger.log(F("[LT] DOOR OPENING"));
                }
                this->openDoor();

                if(pContext->isInAlarm()) {
                    setState(DOOR_CLOSING);
                }

                if(this->isDoorOpen()) {
                    setState(DOOR_OPEN);
                } else {
                    setState(DOOR_OPENING);
                }

                break;
            }

            case DOOR_OPEN: {
                if(this->checkAndSetJustEntered()){
                    Logger.log(F("[LT] DOOR OPEN"));
                }

                if(pContext->isInAlarm()) {
                    setState(DOOR_CLOSING);
                }

                if(pPIR->isDetected()) {
                    setState(DRONE_LANDED);
                } else {
                    setState(DOOR_OPEN);
                }

                break;
            }

            case DRONE_LANDED: {
                if(this->checkAndSetJustEntered()){
                    Logger.log(F("[LT] DRONE LANDED"));
                }

                if(pContext->isInAlarm()) {
                    setState(DOOR_CLOSING);
                }

                if(!this->isDroneNear()) {
                    setState(DOOR_OPEN);
                } else {
                    if(lastTime == 0){
                        lastTime = millis();
                    }
                    if(this->isDroneNear()) {
                        if(millis() - lastTime >= DISTANCE_TIME) {
                            setState(DOOR_CLOSING);
                        }
                    }
                }

                break;
            }

            case DOOR_CLOSING: {
                if(this->checkAndSetJustEntered()){   
                    pContext->setBlinkingOn();    
                    pMotor->on();     
                    Logger.log(F("[LT] DOOR CLOSING"));
                }
                this->closeDoor();

                if(!this->isDoorOpen() && pContext->isInAlarm()) {
                    setState(ALARM);
                } else if(!this->isDoorOpen() && !pContext->isInAlarm()) {
                    setState(IDLE);
                } else {
                    setState(DOOR_CLOSING);
                }

                break;
            }

            case ALARM: {
                if(this->checkAndSetJustEntered()){
                    Logger.log(F("[LT] ALARM"));
                }

                if(!pContext->isInAlarm()){
                    setState(IDLE);
                }

                break;
            }
        }
    }
}

void LandingTask::setState(State s){
    state = s;
    justEntered = true;
}

bool LandingTask::checkAndSetJustEntered(){
    bool bak = justEntered;

    if(justEntered){
        justEntered = false;
    }

    return bak;
}

void LandingTask::closeDoor(){
    long dt = millis() - timeInState;
    pMotor->setPosition(90 - ((dt / TIME_TO_OPEN) * 90));

    if(pMotor->getPosition() == 0){
        pMotor->off();
        pContext->setBlinkingOff();
    }
}

void LandingTask::openDoor(){
    long dt = millis() - timeInState;
    pMotor->setPosition((dt / TIME_TO_OPEN) * 90);

    if(pMotor->getPosition() == 90){
        pMotor->off();
        pContext->setBlinkingOff();
    }
}

bool LandingTask::isDoorOpen(){
    return pMotor->getPosition() == 0;
}

bool LandingTask::isDroneNear(){
    if(pPIR->isDetected()){
        Logger.log(F("[LT] DRONE DETECTED"));
    }
    return pSonar->getDistance() < CLOSING_DISTANCE;
}