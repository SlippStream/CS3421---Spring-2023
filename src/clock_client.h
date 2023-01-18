#ifndef CS3421_NICKZIMANSKI_CLOCK_CLIENT_H
#define CS3421_NICKZIMANSKI_CLOCK_CLIENT_H

enum stateMachineStates
{
    IDLE,
    FETCH,
    STORE,
    CONTEXT_STORE,
    DECODE_AND_REQUEST,
    WAIT,
    HALTED,
};

class stateMachine
{
protected:
    stateMachineStates state;
};

class clockClient : public stateMachine
{
public:
    virtual void doCycleWork() = 0;
    virtual void startTick() = 0;
    virtual bool isMoreCycleWorkNeeded() = 0;
};

#endif