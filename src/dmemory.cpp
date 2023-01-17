#include "dmemory.h"

emulDataMemory::emulDataMemory()
{
    sz = 0;
    wait_ticks = 0;
}

void emulDataMemory::create(int size)
{
    bytes = new uint8_t[size];
    sz = size;
    reset();
}

void emulDataMemory::reset()
{
    for (int i = 0; i < sz; i++)
    {
        bytes[i] = 0x0;
    }
}

void emulDataMemory::dump(uint8_t address, uint8_t count)
{
    std::cout << "Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" << std::endl;
    unsigned int limit = address + count;
    for (unsigned int i = 0; i < limit; i = i + 16)
    {
        if (address > i + 16)
        {
            continue;
        }
        std::cout << "0x" << std::setw(2) << std::setfill('0')
                  << std::uppercase << std::hex << (int)i << ' ';
        for (unsigned int j = 0; j < 16; j++)
        {
            if (i + j < address)
            {
                std::cout << "   ";
                continue;
            }

            if (i + j >= limit)
            {
                break;
            }

            std::cout << std::setw(2) << std::setfill('0') << std::uppercase
                      << std::hex << (int)bytes[j + i] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void emulDataMemory::set(uint8_t address, uint8_t count, std::string bytez[], int startAt)
{
    for (uint8_t i = 0; i < count; i++)
    {
        bytes[i + address] = std::stoul(bytez[i + startAt], nullptr, 16);
    }
}

void emulDataMemory::startFetch(uint8_t *address, unsigned int count, uint8_t *writePtr, bool *donePtr)
{
    state = FETCH;
    op_addressPtr = address;
    op_count = count;
    op_answerPtr = writePtr;
    op_donePtr = donePtr;
    wait_ticks = 0;
}

void emulDataMemory::executeFetch(uint8_t *address, unsigned int count, uint8_t *writePtr, bool *donePtr)
{
    cpy(writePtr, address, count);
    cycleDone = true;
    *donePtr = true;
}

void emulDataMemory::startStore(uint8_t *address, unsigned int count, uint8_t *readPtr, bool *donePtr)
{
    state = STORE;
    op_addressPtr = address;
    op_count = count;
    op_answerPtr = readPtr;
    op_donePtr = donePtr;
    wait_ticks = 0;
}

void emulDataMemory::executeStore(uint8_t *address, unsigned int count, uint8_t *readPtr, bool *donePtr)
{
    cpy(address, readPtr, count);
    cycleDone = true;
    *donePtr = true;
}

void emulDataMemory::cpy(uint8_t *to, uint8_t *from, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        *(to + i) = *(from + i);
    }
}

void emulDataMemory::startTick()
{
    wait_ticks++;
}

void emulDataMemory::doCycleWork()
{
    if (state == IDLE)
    {
        cycleDone = true;
        return;
    }

    /** Fetch / Store */
    if (wait_ticks < 4)
    {
        cycleDone = true;
        return;
    }
    cycleDone = false;

    if (state == FETCH)
    {
        executeFetch(op_addressPtr, op_count, op_answerPtr, op_donePtr);
        state = IDLE;
        return;
    }

    if (state == STORE)
    {
        executeStore(op_addressPtr, op_count, op_answerPtr, op_donePtr);
        state = IDLE;
        return;
    }
}

bool emulDataMemory::isMoreCycleWorkNeeded()
{
    return !cycleDone;
}
