#include "ButtonImpl.h"
#include "Arduino.h"

//serve per far in modo di poter chiamare la funzione buttonPressed() in call()
ButtonImpl* instance = nullptr;

ButtonImpl::ButtonImpl(int pin){
  this->pin = pin;
  this->button = false;
  pinMode(pin, INPUT);
  instance = this;
  attachInterrupt(digitalPinToInterrupt(pin), &call, RISING);
} 

void ButtonImpl::resetButton(){
  button = false;
}
  
bool ButtonImpl::isPressed(){
  return button;
}

void call(){
  if (instance != nullptr) {
    instance ->buttonPressed();
  }
}

void ButtonImpl::buttonPressed(){
  button = true;
}