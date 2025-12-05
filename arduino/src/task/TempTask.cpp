#include "TempTask.h"
#include "config.h"
#include "kernel/Logger.h"
#include "string.h"

long timeLastState;

TempTask::TempTask(Led* pLed, TempSensor* pTempSensor, ButtonImpl* pButton, LiquidCrystal_I2C* plcd, Context* pContext) :
    pContext(pContext), pLed(pLed), pTempSensor(pTempSensor), plcd(plcd), pButton(pButton) {
        setState(IDLE);
}

void TempTask::tick() {
    float temp = pTempSensor->getTemperature();
    switch (state) {
    case IDLE:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("loIDLE-TEMP");
            timeLastState = 0;
        }
        if (temp > TEMP_T1) {
            setState(O_T1);
        }
        break;

    case O_T1:
        Serial.print("lo");
        Serial.println(pTempSensor->getTemperature());
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("loT1-TEMP");
            Logger.log("loTemp: " + (int)pTempSensor->getTemperature());
        }
        if (temp < TEMP_T1) {
            setState(IDLE);
            pContext->setPreAlarmOff();
        } else if (elapsedTimeInState() > T_MAX_3) {
            setState(W_NM);
            timeLastState = 0;
            pContext->setPreAlarmOn();
        } 
        else if (temp > TEMP_T2) {
            timeLastState = elapsedTimeInState();
            setState(O_T2);
        }
        break;

    case O_T2:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("loT2-TEMP");
            Logger.log("loTemp: " + (int)pTempSensor->getTemperature());
        }
        if (temp < TEMP_T2) {
            setState(O_T1);
            timeLastState = timeLastState + elapsedTimeInState();
        } else if (elapsedTimeInState() > T_MAX_4) {
            Serial.println("loSono stato sopra i 35 per piu di 2 secondi");
            setState(W_RS);
            pContext->setAlarmOn();
            pContext->setPreAlarmOff();
            pLed->switchOn();
            pButton->resetButton();
        } else if (elapsedTimeInState() + timeLastState > T_MAX_3) {
            pContext->setPreAlarmOn();
        }
        break;

    case W_NM:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("loWN-TEMP");
            Logger.log("loTemp: " + (int)pTempSensor->getTemperature());
        }
        if (temp > TEMP_T2) {
            setState(O_T2);
        } else if (temp < TEMP_T1) {
            setState(IDLE);
            pContext->setPreAlarmOff();
        }
        break;

    case W_RS:
        if(justEntered) {
            checkAndSetJustEntered();
            Logger.log("loALARM-TEMP");
            Logger.log("loTemp: " + (int)pTempSensor->getTemperature());
            plcd->clear();
            plcd->setCursor(1,1);
            plcd->print("ALARM");
        }
        if (pButton->isPressed()) {
            pContext->setAlarmOff();
            pLed->switchOff();
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