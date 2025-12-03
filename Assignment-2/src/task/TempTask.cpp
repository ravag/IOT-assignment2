#include "TempTask.h"
#include "config.h"
#include "kernel/Logger.h"

long timeLastState;

TempTask::TempTask(Led* pLed, TempSensorTMP36* pTempSensor, ButtonImpl* pButton, Context* pContext) :
    pContext(pContext), pLed(pLed), pTempSensor(pTempSensor), pButton(pButton) {
        setState(D_IN);
}

void TempTask::tick() {
    float temp = pTempSensor->getTemperature();
    switch (state) {
    case IDLE:
        if (pContext->isDroneIn()) {
            setState(D_IN);
        }
        break;
    
    case D_IN:
        if (!pContext->isDroneIn()) {
            setState(IDLE);
        } else if (temp > TEMP_T1) {
            setState(O_T1);
            timeLastState = 0;
        }
        break;

    case O_T1:
        if (temp < TEMP_T1) {
            setState(D_IN);
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
        if (temp < TEMP_T2) {
            setState(O_T1);
            timeLastState = timeLastState + elapsedTimeInState();
        } else if (elapsedTimeInState() > T_MAX_4) {
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
        if (temp > TEMP_T2) {
            setState(O_T2);
        } else if (temp < TEMP_T1) {
            setState(D_IN);
        }
        break;

    case W_RS:
        if (pButton->isPressed()) {
            pContext->setAlarmOff();
            pLed->switchOff();
            setState(D_IN);
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