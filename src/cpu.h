#ifndef CS3421_NICKZIMANSKI_CPU_H
#define CS3421_NICKZIMANSKI_CPU_H
#ifndef CS3421_NICKZIMANSKI_DMEMORY_H
#include "dmemory.h"
#endif
#ifndef CS3421_NICKZIMANSKI_IMEMORY_H
#include "imemory.h"
#endif
#ifndef CS3421_NICKZIMANSKI_CACHE_H
#include "cache.h"
#endif
#include <vector>

enum entropyInstructions
{
    instr_add,
    instr_addi,
    instr_mul,
    instr_inv,
    instr_beq,
    instr_bneq,
    instr_blt,
    instr_lw,
    instr_sw,
    instr_halt
};

enum entropyArgTags
{
    INSTRUCTION = 0,
    DESTINATION = 1,
    SOURCE = 2,
    TARGET = 3,
    IMMEDIATE = 4
};
class emulCpu : public clockClient
{
private:
    uint8_t registers[8];
    uint8_t programCounter;
    uint16_t tickCounter;
    uint16_t lastTicked;
    emulDataMemory *dmemory;
    emulInstrMemory *imemory;
    emulCache *cache;
    entropyInstructions pendingInstruction;
    unsigned int current_instruction;
    uint8_t entropyTokens[5];
    unsigned int wait_ticks = 0;
    bool workToDo;
    bool *donePtr = new bool;

public:
    emulCpu();

    void doCycleWork();
    void startTick();
    bool isMoreCycleWorkNeeded();

    void initialize(emulDataMemory *, emulInstrMemory *, emulCache *);
    void reset();
    void setReg(int, uint8_t);
    void dump();
    void fetch();
    void executeInstruction();
    void executeDelayedInstruction();
    void releaseToIdle();
    void increment();

    void instr_add();
    void instr_addImmediate();
    void instr_multiply();
    void executeMultiply();
    void instr_invert();
    void instr_branchIfEqual();
    void instr_branchIfNotEqual();
    void instr_branchIfLessThan();
    void executeBranch();
    void instr_loadWord();
    void instr_storeWord();
    void instr_halt();

    /**
     * PC is 8
     * rest is 0-7
     * @throws if the given string is not a register
     * @return the index of the given register string in the cpu
     */
    static int
    getRegisterIndex(char const (&)[3]);
    static int twoCpCmp(uint8_t, uint8_t);
    static const int NUM_REG = 8;
};

#endif