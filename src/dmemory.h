#ifndef CS3421_NICKZIMANSKI_DMEMORY_H
#define CS3421_NICKZIMANSKI_DMEMORY_H
#ifndef CS3421_NICKZIMANSKI_CLOCK_CLIENT_H
#include "clock_client.h"
#endif
#include <cstdint>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>

class emulDataMemory : public clockClient
{
private:
    uint8_t op_address;
    uint8_t *op_answerPtr;
    bool *op_validityPtr;
    uint8_t *bytes;
    unsigned int wait_ticks;
    bool cycleDone;
    bool *op_donePtr;
    unsigned int op_count;
    int sz;
    void cpy(uint8_t *, uint8_t *, unsigned int);
    void executeFetch(uint8_t, unsigned int, uint8_t *, bool *);
    void executeStore(uint8_t, unsigned int, uint8_t *, bool *);

public:
    emulDataMemory();

    // Clock Client
    void startTick();
    void doCycleWork();
    bool isMoreCycleWorkNeeded();

    void create(int);
    void reset();
    void dump(uint8_t, uint8_t);
    void set(uint8_t, uint8_t, std::string[], int);
    void startFetch(uint8_t, unsigned int, uint8_t *, bool *);
    void startStore(uint8_t, unsigned int, uint8_t *, bool *);
    void startContextStore(uint8_t, unsigned int, uint8_t *, bool *, bool *);
    void executeContextStore(uint8_t, unsigned int, uint8_t *, bool *, bool *);
};
#endif