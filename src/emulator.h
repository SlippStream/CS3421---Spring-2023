#ifndef CS3421_NICKZIMANSKI_EMULATOR_H
#define CS3421_NICKZIMANSKI_EMULATOR_H
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#ifndef CS3421_NICKZIMANSKI_CLOCK_H
#include "clock.h"
#endif
struct tagged_args;
enum devices
{
    DEV_CPU = 1,
    DEV_DMEMORY = 2,
    /** end clock clients */
    DEV_CLOCK = 3,
    DEV_IMEMORY = 4
};
enum commands
{
    CMD_CREATE,
    CMD_RESET,
    CMD_DUMP,
    CMD_SET,
    CMD_TICK,
};
class emulator
{
private:
    std::vector<clockClient *> clock_clients;

public:
    emulDataMemory *em_dmemory = new emulDataMemory();
    emulInstrMemory *em_imemory = new emulInstrMemory();
    emulClock *em_clock = new emulClock();
    emulCpu *em_cpu = new emulCpu();
    emulator();
    /**
     * Replaces all instances of a given delimeter with null terminator characters
     * @returns the number of tokens found
     */
    struct tagged_args *tokenize(std::string);

    void emulate(std::ifstream &);

    void passCommand(devices, commands, std::string[]);
};

int main(int, char **);

#endif