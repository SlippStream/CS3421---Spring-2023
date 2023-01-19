#ifndef CS3421_NICKZIMANSKI_IODEVICE_H
#define CS3421_NICKZIMANSKI_IODEVICE_H
#ifndef CS3421_NICKZIMANSKI_DMEMORY_H
#include "dmemory.h"
#endif
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>

class emulIODevice : public clockClient
{
private:
    std::ifstream *filestream = new std::ifstream;
    std::stringstream ss;
    uint8_t *reg = new uint8_t;
    uint8_t writeVal;
    bool done = false;
    unsigned int ticks = 0;
    bool initialized = false;
    unsigned int listenTick;
    emulDataMemory *dmemory;

    void read(uint8_t);
    void write(uint8_t, uint8_t);

public:
    emulIODevice();
    void initialize(emulDataMemory *);
    void reset();
    void dump();
    void load(std::string);

    void doCycleWork();
    void startTick();
    bool isMoreCycleWorkNeeded();
};

#endif
