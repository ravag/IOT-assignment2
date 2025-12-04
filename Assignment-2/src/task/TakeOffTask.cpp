#include "TakeOffTask.h"
#include "BlinkingTask.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

TakeOffTask :: TakeOffTask(Context* pContext, Sonar* pSensor, ServoMotor* pMotor, LiquidCrystal_I2C* pLcd, TempSensor* pTemp):
 context(pContext),sensor(pSensor), motor(pMotor), lcd(pLcd), temp(pTemp) {
    setState(IDLE);
    lcd->clear();
    lcd->setCursor(2,1);
    lcd->print("DRONE INSIDE");
    this->alreadyOver = false;
    this->alarm = false;
}

void TakeOffTask::setState(State s) {
    state = s;
    timeInState = millis();
    this->justEntered = true;
}

void TakeOffTask::tick() {
    if (this->context->isInAlarm())
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
            
            Msg* msg = MsgService.receiveMsg();
            if (msg != NULL)
            {
                if (msg->getContent() == "Depart" && !this->context->isInPreAlarm())
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
                Logger.log("[TakeOffTask]: Entered Opening State");
            }
            
            long dt = millis() - timeInState;
            motor->setPosition((dt/TIME_TO_OPEN)*90);

            if (this->motor->getPosition() == 90)
            {
                this->setState(OPEN);
            }
            
            break;

        case OPEN:
            if (justEntered)
            {
                this->motor->off();
                justEntered = false;
                Logger.log("[TakeOffTask]: Entered Open State");
            }

            sensor->setTemperature(temp->getTemperature());
            float dist = sensor->getDistance();
            /*se sei sopra una distaza d per t secondi comincia a chiudere*/
            if (dist > CLOSING_DISTANCE || dist == NO_OBJ_DETECTED) {
                if (alreadyOver) {
                    timePass += millis() - lastTime;
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
            if(justEntered)
            {
                this->motor->on();
                justEntered = false;
                Logger.log("[TakeOffTask]: Entered Closing State");
            }

            long dt = millis() - timeInState;
            motor->setPosition((dt/TIME_TO_OPEN) * 90);

            if (motor->getPosition() == 0)
            {
                this->motor->off();
                context->setBlinkingOff();
                this->setState(this->alarm ? ALARM : IDLE);
            }
            
            break;
        case ALARM:
            if (justEntered)
            {
                justEntered = false;
                Logger.log("[TakeOffTask]: Entered Alarm State");
            }
        
            if (!this->context->isInAlarm())
            {
                this->setState(IDLE);
            }
            
            break;
    }
}