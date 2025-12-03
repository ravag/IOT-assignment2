#include "Context.h"

Context::Context(){
    alarm = false;
}

void Context::setAlarmOff(){
    alarm = false;
}

void Context::setAlarmOn(){
    alarm = true;
}

bool Context::isInAlarm(){
    return alarm;
}