#ifndef CS3421_NICKZIMANSKI_HEX_WORDS_H
#define CS3421_NICKZIMANSKI_HEX_WORDS_H
#include <stdint.h>
#include <iostream>

class wordPair;
class wordContainer;

class wordPair
{
private:
    uint8_t *bytes;
    void validateIndex(uint8_t);

public:
    wordPair();
    unsigned int get(uint8_t);
    void set(uint8_t, unsigned int);
    unsigned int operator[](uint8_t);
};

class wordContainer
{
private:
    wordPair *words;
    unsigned int size;
    wordPair *getPair(unsigned int);

public:
    void push(unsigned int);
    void set(unsigned int, unsigned int);
    unsigned int get(unsigned int);
    wordContainer(unsigned int);
    unsigned int operator[](unsigned int);
    unsigned int getSize();
};

#endif