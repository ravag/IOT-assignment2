#include "Context.h"

Context::Context(){
    alarm = false;
    preAlarm = false;
    droneIn = true;
    blinking = false;
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

void Context::setDroneIn(){
    droneIn = true;
}

void Context::setDroneOut(){
    droneIn = false;
}

void Context::setBlinkingOn(){
    blinking = true;
}

void Context::setBlinkingOff(){
    blinking = false;
}

bool Context::isInPreAlarm(){
    return preAlarm;
}

bool Context::isInAlarm(){
    return alarm;
}

bool Context::isDroneIn(){
    return droneIn;
}

bool Context::isBlinking(){
    return blinking;
}