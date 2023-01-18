#include "cache.h"

emulCache::emulCache()
{
    reset();
}

void emulCache::initialize(emulDataMemory *mem)
{
    dmemory = mem;
}

void emulCache::dump()
{
    std::cout << "CLO        : 0x" << std::setw(2) << std::setfill('0')
              << std::hex << (int)CLO << std::endl;

    std::cout << "cache data : ";
    for (int i = 0; i < 8; i++)
    {
        std::cout << "0x" << std::setw(2) << std::setfill('0')
                  << std::hex << (int)mirror_data[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Flags      : ";
    for (int i = 0; i < 8; i++)
    {
        std::cout << "  " << (char)enumChar(mirror_states[i]) << "  ";
    }
    std::cout << "\n\n";
}

void emulCache::off()
{
    enabled = false;
}

void emulCache::invalidate()
{
    for (int i = 0; i < 8; i++)
    {
        mirror_states[i] = INVALID;
    }
}

void emulCache::reset()
{
    enabled = false;
    CLO = 0;

    invalidate();
    for (int i = 0; i < 8; i++)
    {
        mirror_data[i] = 0;
    }
}

void emulCache::on()
{
    enabled = true;
}

bool emulCache::checkAnyState(cacheByteStates s)
{
    for (int i = 0; i < 8; i++)
    {
        if (mirror_states[i] == s)
            return true;
    }
    return false;
}

void emulCache::setCLO(uint8_t address)
{
    CLO = (address & 0b11111000) >> 3;
    // std::cout << "Setting CLO to " << (int)CLO
    //           << " for address " << (int)address << std::endl;
}

bool emulCache::fetchHit(uint8_t address)
{
    if (address < CLO * 8 || address >= (CLO * 8) + 8)
    {
        return false;
    }
    return mirror_states[address & 0x07] > 0;
}
bool emulCache::storeHit(uint8_t address)
{
    if (address < CLO * 8 || address >= (CLO * 8) + 8)
    {
        return false;
    }
    return true;
}

void emulCache::flush(uint8_t address, uint8_t *readPtr, bool *donePtr)
{
    bool forced = false;
    if (address == 0xFF)
    {
        // Forced flush
        state = CONTEXT_STORE;
        forced = true;
    }
    if (!(checkAnyState(VALID) || checkAnyState(WRITTEN)))
    {
        dmemory->startStore(address, 1, readPtr, donePtr);
        if (!forced)
            setCLO(address);
        mirror_data[address & 0x07] = *readPtr;
        mirror_states[address & 0x07] = WRITTEN;
        state = IDLE;
        return;
    }

    *selfishDonePtr = false;
    waitingAddress = address;
    waitingAnswerPtr = readPtr;
    waitingDonePtr = donePtr;

    bool states[8];
    for (int i = 0; i < 8; i++)
    {
        states[i] = mirror_states[i] == 2;
    }

    dmemory->startContextStore(CLO * 8, 8, &mirror_data[0], &states[0], selfishDonePtr);
    if (!forced)
        state = STORE;
}

void emulCache::requestLine(uint8_t address, uint8_t *writePtr, bool *donePtr)
{
    *selfishDonePtr = false;
    waitingAddress = address;
    waitingAnswerPtr = writePtr;
    waitingDonePtr = donePtr;
    dmemory->startFetch(address, 8, &mirror_data[0], selfishDonePtr);
    state = FETCH;
}

void emulCache::startMemFetch(uint8_t address, uint8_t *writePtr, bool *donePtr)
{
    if (!enabled)
    {
        dmemory->startFetch(address, 1, writePtr, donePtr);
        return;
    }

    if (address == 0xFF)
    {
        invalidate();
        return;
    }

    if (fetchHit(address))
    {
        *writePtr = mirror_data[address & 0x07];
        *donePtr = true;
        return;
    }
    // Cache miss, Request the new line
    requestLine(address, writePtr, donePtr);
}

void emulCache::startMemStore(uint8_t address, uint8_t *readPtr, bool *donePtr)
{
    if (!enabled)
    {
        dmemory->startStore(address, 1, readPtr, donePtr);
        return;
    }

    if (address == 0xFF)
    {
        flush(address, readPtr, donePtr);
        return;
    }

    bool noWritten = !checkAnyState(WRITTEN);
    bool hit = storeHit(address);
    if (hit || noWritten)
    {
        if (noWritten && !hit)
        {
            // We missed the cache, but we'll hold onto this one byte for now
            invalidate();
            setCLO(address);
        }
        mirror_data[address & 0x07] = *readPtr;
        mirror_states[address & 0x07] = WRITTEN;
        *donePtr = true;
        return;
    }
    // Cache miss
    flush(address, readPtr, donePtr);
}

void emulCache::doCycleWork()
{
    if (!enabled)
    {
        return;
    }

    if (!(*selfishDonePtr))
    {
        return;
    }
    *selfishDonePtr = false;

    if (state == STORE)
    {
        setCLO(waitingAddress);
        invalidate();
        mirror_data[waitingAddress & 0x07] = *waitingAnswerPtr;
        mirror_states[waitingAddress & 0x07] = WRITTEN;
        *waitingDonePtr = true;
        state = IDLE;
        return;
    }
    if (state == CONTEXT_STORE)
    {
        for (int i = 0; i < 8; i++)
        {
            if (mirror_states[i] == WRITTEN)
            {
                mirror_states[i] == VALID;
            }
        }
        *waitingDonePtr = true;
        state = IDLE;
        return;
    }
    if (state == FETCH)
    {
        setCLO(waitingAddress);
        for (int i = 0; i < 8; i++)
        {
            mirror_states[i] = VALID;
        }
        *waitingAnswerPtr = mirror_data[waitingAddress & 0x07];
        *waitingDonePtr = true;
        state = IDLE;
        return;
    }
}

char emulCache::enumChar(cacheByteStates s)
{
    switch (s)
    {
    case INVALID:
        return 'I';
    case VALID:
        return 'V';
    case WRITTEN:
        return 'W';
    default:
        return 0;
    }
}