#ifndef __CONTEXT__
#define __CONTEXT__

class Context{

    public: 
        Context();

        void setAlarmOn();
        void setAlarmOff();

        bool isInAlarm();

    private:
        bool alarm;

};

#endif