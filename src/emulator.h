#ifndef CS3421_NICKZIMANSKI_EMULATOR_H
#define CS3421_NICKZIMANSKI_EMULATOR_H
#ifndef CS3421_NICKZIMANSKI_CLOCK_H
#include "clock.h"
#endif
struct tagged_args;
enum devices
{
    DEV_CPU = 1,
    DEV_DMEMORY = 2,
    DEV_IO = 3,
    /** end clock clients */
    DEV_CLOCK = 4,
    DEV_IMEMORY = 5,
    DEV_CACHE = 6
};
enum commands
{
    CMD_CREATE,
    CMD_RESET,
    CMD_DUMP,
    CMD_SET,
    CMD_TICK,
    CMD_ON,
    CMD_OFF,
    CMD_LOAD
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
    emulCache *em_cache = new emulCache();
    emulIODevice *em_io = new emulIODevice();
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