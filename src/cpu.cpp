#include <cpu.h>

cpu::cpu()
{
}

void cpu::doCycleWork()
{
    // TODO
}

void cpu::reset()
{
    // TODO
}

void cpu::setReg(int index, hexByte newVal)
{
    registers[index] = newVal;
}

void cpu::dump()
{
    // TODO
}
/**
 * PC is 8
 * rest is 0-7
 * @throws if the given string is not a register
 * @return the index of the given register string in the cpu
 */
int cpu::getRegisterIndex(char registerName[])
{
    if (sizeof(registerName) != 2)
    {
        throw 1;
    }

    if (registerName == "PC")
    {
        return 8;
    }

    if (registerName[0] != 'R')
    {
        throw 2;
    }
    return registerName[1] - 'A';
}