#include "LandingTask.h"
#include <Arduino.h>
#include "kernel\Logger.h"
#include "kernel/MsgService.h"

static float dist2 = 0.1;

LandingTask::LandingTask(ServoMotor* pMotor, Pir* pPIR, ProximitySensor* pSonar, LiquidCrystal_I2C* pLCD, Context* pContext):
pMotor(pMotor), pPIR(pPIR), pSonar(pSonar), pLCD(pLCD), pContext(pContext) {
    setState(IDLE);
}

void LandingTask::tick(){
    if (!pContext->isDroneIn()) {
        switch(state){
            case IDLE: {
                if(this->checkAndSetJustEntered()){
                    pMotor->setPosition(0);
                    pLCD->clear();
                    pLCD->print("DRONE OUT");
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
                }

                break;
            }

            case DOOR_OPEN: {
                if(this->checkAndSetJustEntered()){
                    Logger.log(F("[LT] DOOR OPEN"));
                    pMotor->off();
                }

                if(pContext->isInAlarm()) {
                    setState(DOOR_CLOSING);
                } else if (pSonar->getDistance() <= dist2) {
                    this->setState(DRONE_LANDED);
                }
                break;
            }

            case DRONE_LANDED: {
                if(this->checkAndSetJustEntered()){
                    Logger.log(F("[LT] DRONE LANDED"));
                    lastTime = millis();
                }
                float dist = pSonar->getDistance();
                /*se sei sopra una distaza d per t secondi comincia a chiudere*/
                Serial.print("lo");
                Serial.println(dist);

                if(pContext->isInAlarm()) {
                    setState(DOOR_CLOSING);
                } else if(pSonar->getDistance() > dist2) {
                    setState(DOOR_OPEN);
                } else if (millis() - lastTime >= DISTANCE_TIME) {
                    this->setState(DOOR_CLOSING);    
                }
                break;
            }

            case DOOR_CLOSING: {
                if(this->checkAndSetJustEntered()){    
                    pMotor->on();     
                    Logger.log(F("[LT] DOOR CLOSING"));
                }

                if(this->isDoorClosed() && pContext->isInAlarm()) {
                    pContext->setBlinkingOff();
                    setState(ALARM);
                } else if(this->isDoorClosed() && !pContext->isInAlarm()) {
                    pContext->setDroneIn();
                    pContext->setBlinkingOff();
                    setState(IDLE);
                } else {
                    this->closeDoor();
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
    timeInState = millis();
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
    currentPosition = dt / TIME_TO_OPEN;
    pMotor->setPosition(90 - (currentPosition * 90));

    if(pMotor->getPosition() <= 0){
        pMotor->off();
    }
}

void LandingTask::openDoor(){
    long dt = millis() - timeInState;
    currentPosition = dt / TIME_TO_OPEN;
    pMotor->setPosition(currentPosition * 90);

    if(pMotor->getPosition() >= 90){
        pMotor->off();
    }
}

bool LandingTask::isDoorOpen(){
    return pMotor->getPosition() >= 90;
}

bool LandingTask::isDoorClosed(){
    return pMotor->getPosition() <= 0;
}

bool LandingTask::isDroneNear(){
    if(pPIR->isDetected()){
        Logger.log(F("[LT] DRONE DETECTED"));
        return true;
    } else {
        Logger.log("[LT] DRONE NOT DETECTED");
        return false;
    }
}