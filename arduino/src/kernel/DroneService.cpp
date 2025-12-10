#include "DroneService.h"
#include "MsgService.h"

void DroneService::droneMessage(const String& msg){
    MsgService.sendMsg("dr"+msg);
}



