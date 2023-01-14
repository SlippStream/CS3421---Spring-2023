#include "clock.h"

emulClock::emulClock()
{
    ticks = 0;
}

void emulClock::reset()
{
    ticks = 0;
}

void emulClock::tick(int tks, const std::vector<clockClient *> &clients)
{
    if (tks < 0)
    {
        throw 1;
    }

    for (int i = 0; i < tks; i++)
    {
        for (auto it = clients.begin(); it != clients.end(); ++it)
        {
            (*it)->doCycleWork();
        }
        ticks = ticks + 1;
    }
}

void emulClock::dump()
{
    std::cout << "Clock: " << ticks << std::endl
              << std::endl;
}