#ifndef __DRONESERVICE__
#define __DRONESERVICE__

#include "Arduino.h"


class DroneService {
    
public: 
  
  void droneMessage(const String& msg);
};

extern DroneService droneService;

#endif

