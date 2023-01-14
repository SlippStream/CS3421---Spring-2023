#ifndef CS3421_NICKZIMANSKI_CPU_H
#define CS3421_NICKZIMANSKI_CPU_H
#ifndef CS3421_NICKZIMANSKI_CLOCK_CLIENT_H
#include "clock_client.h"
#endif
#ifndef CS3421_NICKZIMANSKI_MEMORY_H
#include "memory.h"
#endif
#include <vector>
#include <iostream>

class emulCpu : public clockClient
{
private:
    uint8_t registers[8];
    uint8_t programCounter;
    emulMemory *memory;

public:
    emulCpu();
    void initialize(emulMemory *mem);
    void doCycleWork();
    void reset();
    void setReg(int, uint8_t);
    void dump();
    /**
     * PC is 8
     * rest is 0-7
     * @throws if the given string is not a register
     * @return the index of the given register string in the cpu
     */
    static int getRegisterIndex(char const (&)[3]);
    static const int NUM_REG = 8;
};

#endif