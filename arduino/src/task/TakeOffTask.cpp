#include "TakeOffTask.h"
#include "BlinkingTask.h"
#include "kernel/Logger.h"


TakeOffTask :: TakeOffTask(Context* pContext, Sonar* pSensor, ServoMotor* pMotor, LiquidCrystal_I2C* pLcd, TempSensor* pTemp):
 context(pContext),sensor(pSensor), motor(pMotor), lcd(pLcd), temp(pTemp) {
    setState(IDLE);
    this->alreadyOver = false;
    this->alarm = false;
}

void TakeOffTask::setState(State s) {
    state = s;
    timeInState = millis();
    this->justEntered = true;
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
            if (justEntered)
            {
                justEntered = false;
                Logger.log("[TakeOffTask]: Entered Idle State");
            }
            
            msg = MsgService.receiveMsg();
            if (msg != NULL)
            {
                if (msg->getContent() == "rtk" && !this->context->isInPreAlarm())
                {
                    this->setState(OPENING);
                    this->context->setBlinkingOn();
                    lcd->clear();
                    lcd->setCursor(2,1);
                    lcd->print("TAKE OFF");
                }
                
            }
            
            break;

        case OPENING:
            if (justEntered)
            {
                this->motor->on();
                justEntered = false;
                Logger.log("lo[TakeOffTask]: Entered Opening State");
            }
            
            dt = millis() - timeInState;
            motor->setPosition(((float)(dt/TIME_TO_OPEN))*90);

            if (this->motor->getPosition() >= 90)
            {
                this->setState(OPEN);
            }
            
            break;

        case OPEN:
            if (justEntered)
            {
                this->motor->off();
                justEntered = false;
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
                        Logger.log("lo[TakeOffTask]: time passed -> " + timePass);
                        this->setState(CLOSING);
                        lcd->clear();
                        lcd->setCursor(2,1);
                        lcd->print("DRONE OUT");
                        this->context->setDroneOut();
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
            if(justEntered)
            {
                this->motor->on();
                justEntered = false;
                Logger.log("lo[TakeOffTask]: Entered Closing State");
            }

            dt = millis() - timeInState;
            motor->setPosition( 90 - ((float)(dt/TIME_TO_OPEN) * 90));

            if (motor->getPosition() == 0)
            {
                this->motor->off();
                context->setBlinkingOff();
                this->setState(this->context->isInAlarm() ? ALARM : IDLE);
            }
            
            break;
        case ALARM:
            if (justEntered)
            {
                justEntered = false;
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