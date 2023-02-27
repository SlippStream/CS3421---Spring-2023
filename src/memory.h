#ifndef CS3421_NICKZIMANSKI_MEMORY_H
#define CS3421_NICKZIMANSKI_MEMORY_H
#include <cstdint>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>

class emulMemory
{
private:
    uint8_t *bytes;
    int sz;

public:
    emulMemory();

    void create(int);
    void reset();
    void dump(uint8_t, uint8_t);
    void set(uint8_t, uint8_t, std::string[], int);
    uint8_t fetch(uint8_t);
};
#endif