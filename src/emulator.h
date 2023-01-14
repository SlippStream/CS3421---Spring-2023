#ifndef CS3421_NICKZIMANSKI_EMULATOR_H
#define CS3421_NICKZIMANSKI_EMULATOR_H
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include "memory.h"
#include "cpu.h"
#include "clock.h"

class emulator
{
private:
    std::vector<clockClient *> clock_clients;

public:
    emulMemory *em_memory = new emulMemory();
    emulClock *em_clock = new emulClock();
    emulCpu *em_cpu = new emulCpu();
    emulator();
    /**
     * Replaces all instances of a given delimeter with null terminator characters
     * @returns the number of tokens found
     */
    std::vector<std::string> tokenize(std::string);

    void emulate(std::ifstream &);

    void passCommand(std::string, std::string, std::string[]);
};

int main(int, char **);

#endif