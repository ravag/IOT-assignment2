#ifndef __CONTEXT__
#define __CONTEXT__

class Context{

    public: 
        Context();

        void setAlarmOn();
        void setAlarmOff();
        void setPreAlarmOn();
        void setPreAlarmOff();

        bool isInAlarm();
        bool isInPreAlarm();

    private:
        bool alarm;
        bool preAlarm;

};

#endif