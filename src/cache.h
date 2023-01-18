#ifndef CS3421_NICKZIMANSKI_CACHE_H
#define CS3421_NICKZIMANSKI_CACHE_H
#ifndef CS3421_NICKZIMANSKI_DMEMORY_H
#include "dmemory.h"
#endif

enum cacheByteStates
{
    INVALID = 0, // Does not necessarily match memory
    VALID = 1,   // Matches memory
    WRITTEN = 2, // Is more up-to-date than memory
};

class emulCache : public stateMachine
{
private:
    bool enabled;
    uint8_t CLO;
    emulDataMemory *dmemory;
    uint8_t mirror_data[8];
    cacheByteStates mirror_states[8];
    bool *selfishDonePtr = new bool;
    uint8_t waitingAddress;
    uint8_t *waitingAnswerPtr;
    bool *waitingDonePtr;

    void flush(uint8_t, uint8_t *, bool *);
    void requestLine(uint8_t, uint8_t *, bool *);

    void setCLO(uint8_t);

    void invalidate();
    bool checkAnyState(cacheByteStates);
    bool fetchHit(uint8_t);
    bool storeHit(uint8_t);
    char enumChar(cacheByteStates);

public:
    emulCache();
    void on();
    void off();
    void dump();
    void reset();
    void initialize(emulDataMemory *);

    void doCycleWork();

    void startMemFetch(uint8_t, uint8_t *, bool *);
    void startMemStore(uint8_t, uint8_t *, bool *);
};

#endif