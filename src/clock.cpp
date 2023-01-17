#include "clock.h"

emulClock::emulClock()
{
    reset();
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

    bool workToDo;
    for (int i = 0; i < tks; i++)
    {
        // std::cout << "Tick " << std::dec << ticks << std::endl;
        for (auto it = clients.begin(); it != clients.end(); ++it)
        {
            (*it)->startTick();
        }

        workToDo = true;
        while (workToDo)
        {
            workToDo = false;
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                (*it)->doCycleWork();
                workToDo = workToDo || (*it)->isMoreCycleWorkNeeded();
            }
        }
        ticks = ticks + 1;
    }
}

void emulClock::dump()
{
    std::cout << "Clock: " << std::dec << ticks << std::endl
              << std::endl;
}