#ifndef CPU_H
#define CPU_H
#ifndef HEX_H
#include <hex.h>
#endif

class cpu
{
private:
    hexByte PC;
    hexByte registers[8];

public:
    cpu();

    void doCycleWork();

    void reset();

    void setReg(int, hexByte);

    void dump();

    static int getRegisterIndex(char[]);
};

#endif