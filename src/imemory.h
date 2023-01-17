#ifndef CS3421_NICKZIMANSKI_IMEMORY_H
#define CS3421_NICKZIMANSKI_IMEMORY_H
#ifndef CS3421_NICKZIMANSKI_HEX_WORDS_H
#include "hex_words.h"
#endif
#include <string>
#include <iostream>
#include <iomanip>

class emulInstrMemory
{
private:
    wordContainer *words;
    unsigned int sz;

public:
    emulInstrMemory();
    void create(unsigned int);
    void reset();
    void dump(unsigned int, unsigned int);
    void set(unsigned int, wordContainer *);
    unsigned int load(unsigned int);
};

#endif