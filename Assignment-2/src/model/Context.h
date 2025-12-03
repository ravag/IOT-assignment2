#ifndef __CONTEXT__
#define __CONTEXT__

class Context{

    public: 
        Context();

        void setAlarmOn();
        void setAlarmOff();
        void setPreAlarmOn();
        void setPreAlarmOff();
        void setDroneIn();
        void setDroneOut();

        bool isInAlarm();
        bool isInPreAlarm();
        bool isDroneIn();

    private:
        bool alarm;
        bool preAlarm;
        bool droneIn;

};

#endif