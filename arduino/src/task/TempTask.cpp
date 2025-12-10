#include "TempTask.h"
#include "config.h"
#include "kernel/Logger.h"
#include "kernel/DroneService.h"
#include "string.h"

long timeLastState;

TempTask::TempTask(Led* pLed, TempSensor* pTempSensor, ButtonImpl* pButton, LiquidCrystal_I2C* plcd, Context* pContext) :
     pLed(pLed), pTempSensor(pTempSensor), pButton(pButton), plcd(plcd), pContext(pContext) {
        setState(IDLE);
}

void TempTask::tick() {
    float temp = pTempSensor->getTemperature();
    switch (state) {
    case IDLE:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("[TEMP]: IDLE");
            timeLastState = 0;
        }
        if (temp > TEMP_T1) {
            setState(O_T1);
        }
        break;

    case O_T1:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("[TEMP]: OVER TEMP 1 (30 degrees)");
        }
        if (temp < TEMP_T1) {
            setState(IDLE);
            pContext->setPreAlarmOff();
        } else if (elapsedTimeInState() > T_MAX_3) {
            setState(W_NM);
            timeLastState = 0;
            pContext->setPreAlarmOn();
            Logger.log("[TEMP]: PRE ALARM");
            plcd->clear();
            plcd->setCursor(2,1);
            plcd->print("PRE ALARM");
        } 
        else if (temp > TEMP_T2) {
            timeLastState = elapsedTimeInState();
            setState(O_T2);
        }
        break;

    case O_T2:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("[TEMP]: OVER TEMP 2 (33 degrees)");
        }
        if (temp < TEMP_T2) {
            setState(O_T1);
            timeLastState = timeLastState + elapsedTimeInState();
        } else if (elapsedTimeInState() > T_MAX_4) {
            setState(W_RS);
            pContext->setPreAlarmOff();
            pContext->setAlarmOn();
            plcd->clear();
            plcd->setCursor(2,1);
            plcd->print("ALARM");
            pLed->switchOn();
            pButton->resetButton();
        } else if (elapsedTimeInState() + timeLastState > T_MAX_3) {
            pContext->setPreAlarmOn();
            Logger.log("[TEMP]: PRE ALARM");
            plcd->clear();
            plcd->setCursor(2,1);
            plcd->print("PRE ALARM");
        }
        break;

    case W_NM:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("[TEMP]: WAITING NORMAL TEMP");
        }
        if (temp > TEMP_T2) {
            setState(O_T2);
        } else if (temp < TEMP_T1) {
            setState(IDLE);
            if (pContext->isInPreAlarm()) {
                pContext->setPreAlarmOff();
                plcd->clear();
                plcd->setCursor(2,1);
                if(pContext->isDroneIn()) {
                    plcd->print("DRONE INSIDE");
                } else {
                    plcd->print("DRONE OUTSIDE");
                }
            }
        }
        break;

    case W_RS:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("[TEMP]: ALARM");
            if (!pContext->isDroneIn()) {
                droneService.droneMessage("ALARM!!"); //Landing in NOT possible! ");
            }
        }
        if (pButton->isPressed()) {
            pContext->setAlarmOff();
            pLed->switchOff();
            if (!pContext->isDroneIn()) {
                droneService.droneMessage("no alarm!"); // Landing is now POSSIBLE! We are waiting for you :)");
            }
            setState(IDLE);
        }
        break;
    }
}

void TempTask::setState(State s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long TempTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool TempTask::checkAndSetJustEntered() {
    bool bak = justEntered;
    if (justEntered) {
        justEntered = false;
    }
    return bak;
}