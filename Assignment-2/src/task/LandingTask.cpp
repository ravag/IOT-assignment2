#include "Task\LandingTask.h"
#include "config.h"
#include <Arduino.h>
#include "kernel\Logger.h"

LandingTask::LandingTask(ServoMotor* pMotor, PresenceSensor* pPIR, ProximitySensor* pSonar, LiquidCrystal_I2C* pLCD, Context* pContext):
pMotor(pMotor), pPIR(pPIR), pSonar(pSonar), pLCD(pLCD), pContext(pContext) {
    setState(IDLE);
}

void LandingTask::tick(){
    switch(state){
        case IDLE: {
            if(this->checkAndSetJustEntered()){
                pMotor->closeDoor();
                pLCD->print("DRONE OUT");
                Logger.log(F("[BT] IDLE"));
            }

            if(pContext->isInAlarm) {
                setState(ALARM);
            }

            if(!pContext->isInPreAlarm() && this->recievedSignal && this->droneIsNear()) {
                pLCD->print("LANDING")
                pContext->setBlinkingOn();
                setState(DOOR_OPENING);
            }

            break;
        }

        case DOOR_OPENING: {
            if(this->checkAndSetJustEntered()){
                this->openDoor();
                Logger.log(F("[BT] DOOR OPENING"));
            }

            if(pContext->isInAlarm) {
                setState(ALARM);
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
                Logger.log(F("[BT] DOOR OPEN"));
            }

            if(pContext->isInAlarm) {
                setState(ALARM);
            }

            if(this->isDroneNear()) {
                setState(DRONE_LANDED);
            } else {
                setState(DOOR_OPEN);
            }

            break;
        }

        case DRONE_LANDED: {
            if(this->checkAndSetJustEntered()){
                Logger.log(F("[BT] DRONE LANDED"));
            }

            if(pContext->isInAlarm) {
                setState(ALARM);
            }

            if(!this->isDroneNear()) {
                setState(DOOR_OPEN);
            } else {
                if(lastTime == 0){
                    lastTime = millis();
                }
                if(this->isDroneNear()) {
                    if(millis() - lastTime >= TIME2) {
                        setState(DOOR_CLOSING);
                    }
                }
            }
        }
    }
}

/*
    DA AGGIUNGERE
    closeDoor()
    openDoor()
    isDoorOpen()
    isDroneNear()

*/