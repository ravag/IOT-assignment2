#include "TakeOffTask.h"
#include "BlinkingTask.h"
#include "kernel/Logger.h"


TakeOffTask :: TakeOffTask(Context* pContext, Sonar* pSensor, ServoMotor* pMotor, LiquidCrystal_I2C* pLcd, TempSensor* pTemp):
 context(pContext),sensor(pSensor), motor(pMotor), lcd(pLcd), temp(pTemp) {
    setState(IDLE);
    this->alreadyOver = false;
    this->alarm = false;
}

bool TakeOffTask::checkAndSetJustEntered(){
    bool bak = justEntered;

    if(justEntered){
        justEntered = false;
    }

    return bak;
}

void TakeOffTask::closeDoor(){
    long dt = millis() - timeInState;
    currentPosition = dt / TIME_TO_OPEN;
    motor->setPosition(90 - (currentPosition * 90));
}

void TakeOffTask::openDoor(){
    long dt = millis() - timeInState;
    currentPosition = dt / TIME_TO_OPEN;
    motor->setPosition(currentPosition * 90);
}

void TakeOffTask::setState(State s) {
    state = s;
    this->justEntered = true;
    timeInState = millis();
}

void TakeOffTask::tick() {
    if (this->context->isDroneIn())
    {
    
        if (this->context->isInAlarm() && !alarm)
        {
            this->setState(CLOSING);
            this->alarm = true;
        }
        
        switch (this->state)
        {
            case IDLE:
                if (checkAndSetJustEntered())
                {
                    motor->setPosition(0);
                    Logger.log("[TakeOffTask]: Entered Idle State");
                    lcd->clear();
                    lcd->print("DRONE INSIDE");
                }
                
                msg = MsgService.receiveMsg();
                if (msg != NULL)
                {
                    if (msg->getContent() == "rtk" && !this->context->isInPreAlarm())
                    {
                        this->context->setBlinkingOn();
                        lcd->clear();
                        lcd->setCursor(2,1);
                        lcd->print("TAKE OFF");
                        this->setState(OPENING);
                    }
                    
                }
                
                break;

            case OPENING:
                if (checkAndSetJustEntered())
                {
                    this->motor->on();
                    Logger.log("lo[TakeOffTask]: Entered Opening State");
                }

                openDoor();


                if (this->motor->getPosition() >= 90)
                {
                    this->setState(OPEN);
                }
                
                break;

            case OPEN:
                if (checkAndSetJustEntered())
                {
                    this->motor->off();
                    Logger.log("lo[TakeOffTask]: Entered Open State");
                    alreadyOver = false;
                }

                sensor->setTemperature(temp->getTemperature());
                dist = sensor->getDistance();
                /*se sei sopra una distaza d per t secondi comincia a chiudere*/
                Serial.print("lo");
                Serial.println(dist);
                if (dist > CLOSING_DISTANCE || dist == NO_OBJ_DETECTED) {
                    if (alreadyOver) {
                        timePass = millis() - lastTime;
                        if (timePass > DISTANCE_TIME) {
                            this->setState(CLOSING);
                            lcd->clear();
                            lcd->setCursor(2,1);
                            lcd->print("DRONE OUT");
                        }
                        
                    } else {
                        timePass = 0;
                        lastTime = millis();
                        alreadyOver = true;
                    }
                    
                } else {
                    alreadyOver = false;
                }
                
                break;

            case CLOSING:
                if(checkAndSetJustEntered())
                {
                    this->motor->on();
                    Logger.log("lo[TakeOffTask]: Entered Closing State");
                }

                

                if (motor->getPosition() <= 0)
                {
                    this->motor->off();
                    context->setBlinkingOff();
                    this->setState(this->context->isInAlarm() ? ALARM : IDLE);
                    if (!this->context->isInAlarm())
                    {
                        this->context->setDroneOut();
                    }
                    
                } else {
                    closeDoor();
                }
                
                break;
            case ALARM:
                if (checkAndSetJustEntered())
                {
                    Logger.log("lo[TakeOffTask]: Entered Alarm State");
                }
            
                if (!this->context->isInAlarm())
                {
                    this->setState(IDLE);
                    this->alarm = false;
                }
                
                break;
        }
    }
}