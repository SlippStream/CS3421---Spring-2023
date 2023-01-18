#include "cpu.h"

emulCpu::emulCpu()
{
    reset();
    *donePtr = false;
    workToDo = false;
    state = IDLE;
    wait_ticks = 0;
}

void emulCpu::initialize(emulDataMemory *dmem, emulInstrMemory *imem, emulCache *che)
{
    dmemory = dmem;
    imemory = imem;
    cache = che;
    cache->initialize(dmemory);
}

void emulCpu::startTick()
{
    if (state == HALTED)
        return;

    workToDo = true;
    tickCounter++;

    if (state == IDLE)
    {
        state = FETCH;
    }
}

void emulCpu::doCycleWork()
{
    if (state == HALTED)
    {
        workToDo = false;
        return;
    }

    if (state == FETCH)
    {
        fetch();
    }

    if (state == DECODE_AND_REQUEST)
    {
        executeInstruction();
    }

    if (lastTicked != tickCounter)
    {
        lastTicked = tickCounter;
        cache->doCycleWork();
        if (state == WAIT)
        {
            workToDo = false;
            if (wait_ticks > 0)
            {
                wait_ticks--;
                if (wait_ticks != 0)
                {
                    return;
                }
                executeDelayedInstruction();
            }
            else if (!(*donePtr))
            {
                return;
            }

            releaseToIdle();
        }
    }
}

void emulCpu::fetch()
{
    current_instruction = imemory->load(programCounter);
    state = DECODE_AND_REQUEST;
}

void emulCpu::executeInstruction()
{

    entropyTokens[0] = (uint8_t)((current_instruction & 0xE0000) >> 17); // Instruction
    entropyTokens[1] = (uint8_t)((current_instruction & 0x1C000) >> 14); // Destination
    entropyTokens[2] = (uint8_t)((current_instruction & 0x03800) >> 11); // Source
    entropyTokens[3] = (uint8_t)((current_instruction & 0x00700) >> 8);  // Target
    entropyTokens[4] = (uint8_t)(current_instruction & 0x000FF);         // Immediate

    state = WAIT;
    switch (entropyTokens[0])
    {
    case 0b000:
        // add
        instr_add();
        break;
    case 0b001:
        // addi
        instr_addImmediate();
        break;
    case 0b010:
        // mul
        instr_multiply();
        break;
    case 0b011:
        // inv
        instr_invert();
        break;
    case 0b100:
    {
        switch (entropyTokens[1])
        {
        case 0b000:
            // beq
            instr_branchIfEqual();
            break;
        case 0b001:
            // bneq
            instr_branchIfNotEqual();
            break;
        case 0b010:
            // blt
            instr_branchIfLessThan();
            break;
        }
        break;
    }
    break;
    case 0b101:
        // lw
        instr_loadWord();
        break;
    case 0b110:
        // sw
        instr_storeWord();
        break;
    case 0b111:
        instr_halt();
        break;
    }
}

void emulCpu::executeDelayedInstruction()
{
    switch (pendingInstruction)
    {
    case instr_mul:
        executeMultiply();
        break;
    case instr_beq:
    case instr_bneq:
    case instr_blt:
        executeBranch();
        break;
    default:
        break;
    }
}

bool emulCpu::isMoreCycleWorkNeeded()
{
    return workToDo;
}

void emulCpu::reset()
{
    programCounter = 0;
    tickCounter = 0;
    for (int i = 0; i < NUM_REG; i++)
    {
        registers[i] = 0;
    }
    state = IDLE;
}

void emulCpu::setReg(int index, uint8_t newVal)
{
    if (index == NUM_REG)
    {
        programCounter = newVal;
        state = FETCH;
        return;
    }
    registers[index] = newVal;
}

void emulCpu::dump()
{
    std::cout << "PC: 0x" << std::setw(2) << std::setfill('0')
              << std::uppercase << std::hex << (int)programCounter << std::endl;

    for (int i = 0; i < NUM_REG; i++)
    {
        std::cout << "R" << (char)(i + 'A')
                  << ": 0x" << std::setw(2) << std::setfill('0')
                  << std::uppercase << std::hex << (int)registers[i] << std::endl;
    }

    std::cout << "TC: " << std::dec << (int)tickCounter << std::endl
              << std::endl;
}
int emulCpu::getRegisterIndex(char const (&registerName)[3])
{
    if (registerName[0] == 'P' && registerName[1] == 'C')
    {
        return 8;
    }

    if (registerName[0] != 'R')
    {
        throw 0;
    }

    return registerName[1] - 'A';
}

void emulCpu::releaseToIdle()
{
    programCounter++;
    state = FETCH;
    workToDo = false;
    *donePtr = false;
}

// This instruction takes 1 tick
void emulCpu::instr_add()
{
    registers[entropyTokens[DESTINATION]] = registers[entropyTokens[SOURCE]] + registers[entropyTokens[TARGET]];
    releaseToIdle();
}
// This instruction takes 1 tick
void emulCpu::instr_addImmediate()
{
    registers[entropyTokens[DESTINATION]] = registers[entropyTokens[SOURCE]] + entropyTokens[IMMEDIATE];
    releaseToIdle();
}
// This instruction takes 2 ticks
void emulCpu::instr_multiply()
{
    pendingInstruction = instr_mul;
    wait_ticks = 1;
    state = WAIT;
    workToDo = false;
}
void emulCpu::executeMultiply()
{
    uint8_t temp = registers[entropyTokens[SOURCE]];
    registers[entropyTokens[DESTINATION]] = ((temp & 0xF0) >> 4) * (temp & 0x0F);
    releaseToIdle();
}
void emulCpu::instr_invert()
{
    registers[entropyTokens[DESTINATION]] = ~registers[entropyTokens[SOURCE]];
    releaseToIdle();
}
void emulCpu::instr_branchIfEqual()
{
    if (registers[entropyTokens[SOURCE]] == registers[entropyTokens[TARGET]])
    {
        // Take branch
        wait_ticks = 1;
        state = WAIT;
        pendingInstruction = instr_beq;
        workToDo = false;
        return;
    }
    releaseToIdle();
}
void emulCpu::executeBranch()
{
    programCounter = entropyTokens[IMMEDIATE] - 1;
    releaseToIdle();
}
void emulCpu::instr_branchIfNotEqual()
{
    if (registers[entropyTokens[SOURCE]] != registers[entropyTokens[TARGET]])
    {
        // Take branch
        wait_ticks = 1;
        state = WAIT;
        pendingInstruction = instr_bneq;
        workToDo = false;
        return;
    }
    releaseToIdle();
}
void emulCpu::instr_branchIfLessThan()
{
    if (twoCpCmp(registers[entropyTokens[SOURCE]], registers[entropyTokens[TARGET]]) < 0)
    {
        // Take branch
        wait_ticks = 1;
        state = WAIT;
        pendingInstruction = instr_blt;
        workToDo = false;
        return;
    }
    releaseToIdle();
}
// This instruction takes 5 ticks on cache miss
void emulCpu::instr_loadWord()
{
    *donePtr = false;
    cache->startMemFetch(registers[entropyTokens[TARGET]], &registers[entropyTokens[DESTINATION]], donePtr);
    // dmemory->startFetch(registers[entropyTokens[TARGET]], 1, &registers[entropyTokens[DESTINATION]], donePtr);
    workToDo = false;
    wait_ticks = 0;
    state = WAIT;
}

// This instruction takes 5 ticks
void emulCpu::instr_storeWord()
{
    *donePtr = false;
    cache->startMemStore(registers[entropyTokens[TARGET]], &registers[entropyTokens[SOURCE]], donePtr);
    // dmemory->startStore(registers[entropyTokens[TARGET]], 1, &registers[entropyTokens[SOURCE]], donePtr);
    workToDo = false;
    wait_ticks = 0;
    state = WAIT;
}

void emulCpu::instr_halt()
{
    state = HALTED;
    increment();
    workToDo = false;
}

void emulCpu::increment()
{
    programCounter++;
}

int emulCpu::twoCpCmp(uint8_t a, uint8_t b)
{
    if (a == b)
        return 0;

    if ((a & 0b10000000) == (b & 0b10000000))
        return a - b;

    if ((a & 0b10000000) == 1)
        return a;

    return b;
}