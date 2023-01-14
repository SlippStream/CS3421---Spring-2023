#include "cpu.h"

emulCpu::emulCpu()
{
    reset();
}

void emulCpu::initialize(emulMemory *mem)
{
    memory = mem;
}

void emulCpu::doCycleWork()
{
    // SHIFT
    for (int i = NUM_REG - 1; i > 0; i--)
    {
        registers[i] = registers[i - 1];
    }

    // FETCH
    registers[0] = memory->fetch(programCounter);

    // INCREMENT
    programCounter++;
}

void emulCpu::reset()
{
    programCounter = 0;
    for (int i = 0; i < NUM_REG; i++)
    {
        registers[i] = 0;
    }
}

void emulCpu::setReg(int index, uint8_t newVal)
{
    if (index == NUM_REG)
    {
        programCounter = newVal;
        return;
    }
    registers[index] = newVal;
}

void emulCpu::dump()
{
    std::cout << "PC: " << std::setw(2) << std::setfill('0')
              << std::uppercase << std::hex << (int)programCounter << std::endl;

    for (int i = 0; i < NUM_REG; i++)
    {
        std::cout << "R" << (char)(i + 'A')
                  << ": " << std::setw(2) << std::setfill('0')
                  << std::uppercase << std::hex << (int)registers[i] << std::endl;
    }
    std::cout << std::endl;
}
int emulCpu::getRegisterIndex(char const (&registerName)[3])
{
    if (registerName == "PC")
    {
        return 8;
    }

    if (registerName[0] != 'R')
    {
        throw 0;
    }

    return registerName[1] - 'A';
}