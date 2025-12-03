#ifndef __BUTTONIMPL__
#define __BUTTONIMPL__

#include "Button.h"

class ButtonImpl: public Button {
 
public: 
  ButtonImpl(int pin);
  bool isPressed();
  void resetButton();
  void buttonPressed();

private:

  int pin;
  bool button;

};

#endif