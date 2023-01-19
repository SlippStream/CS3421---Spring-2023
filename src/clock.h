#ifndef CS3421_NICKZIMANSKI_CLOCK_H
#define CS3421_NICKZIMANSKI_CLOCK_H
#ifndef CS3421_NICKZIMANSKI_CPU_H
#include "cpu.h"
#include "IODevice.h"
#endif

class emulClock
{
private:
    unsigned short ticks;

public:
    emulClock();

    void reset();

    void tick(int, const std::vector<clockClient *> &);

    void dump();
};

#endif