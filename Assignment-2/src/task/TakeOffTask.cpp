#include "TakeOffTask.h"
#include "BlinkingTask.h"
#include "kernel/MsgService.h"

TakeOffTask :: TakeOffTask(Context* pContext, Sonar* pSensor, ServoMotor* pMotor, LiquidCrystal_I2C* pLcd, TempSensor* pTemp):
 context(pContext),sensor(pSensor), motor(pMotor), lcd(pLcd), temp(pTemp) {
    setState(IDLE);
    lcd->clear();
    lcd->setCursor(2,1);
    lcd->print("DRONE INSIDE");
    this->alreadyOver = false;
}

void TakeOffTask::setState(State s) {
    state = s;
}

void TakeOffTask::tick() {
    switch (this->state)
    {
        case IDLE:
            Msg* msg = MsgService.receiveMsg();
            if (msg != NULL)
            {
                if (msg == "Depart")
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
            motor->on();
            motor->setPosition(/*calculate position (dt/TIME_TO_OPEN)*90 */);
            /*inserisci nel motore un metodo per ottenere la sua posizione*/
            break;

        case OPEN:
            motor->off();
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
            motor->on();
            motor->setPosition(/*calcola posizione in base al tempo*/);

            if (/*angolo motore == 0 oppure crea metodo is closed*/)
            {
                motor->off();
                context->setBlinkingOff();
                this->setState(IDLE);
            }
            
            break;
        
    }
}