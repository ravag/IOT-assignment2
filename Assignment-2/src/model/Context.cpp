#include "Context.h"

Context::Context(){
    alarm = false;
    preAlarm = false;
}

void Context::setAlarmOff(){
    alarm = false;
}

void Context::setAlarmOn(){
    alarm = true;
}

void Context::setPreAlarmOff(){
    preAlarm = false;
}

void Context::setPreAlarmOn(){
    preAlarm = true;
}

bool Context::isInPreAlarm(){
    return preAlarm;
}

bool Context::isInAlarm(){
    return alarm;
}