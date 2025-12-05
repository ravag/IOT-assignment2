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
        void setBlinkingOn();
        void setBlinkingOff();

        bool isInAlarm();
        bool isInPreAlarm();
        bool isDroneIn();
        bool isBlinking();

    private:
        bool alarm;
        bool preAlarm;
        bool droneIn;
        bool blinking;

};

#endif