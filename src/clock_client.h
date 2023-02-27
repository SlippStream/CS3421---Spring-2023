#ifndef CS3421_NICKZIMANSKI_CLOCK_CLIENT_H
#define CS3421_NICKZIMANSKI_CLOCK_CLIENT_H

enum stateMachineStates
{
    IDLE,
    FETCH,
    STORE,
    DECODE_AND_REQUEST,
    WAIT,
    HALTED,
};

class clockClient
{
public:
    virtual void doCycleWork() = 0;
    virtual void startTick() = 0;
    virtual bool isMoreCycleWorkNeeded() = 0;
};

class stateMachine : public clockClient
{
protected:
    stateMachineStates state;
};

#endif