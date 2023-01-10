#ifndef MEMORY_H
#define MEMORY_H
#ifndef HEX_H
#include <hex.h>
#endif

class memory
{

public:
    void create(int);

    void reset();

    void dump(hexByte, hexByte);

    void set(hexByte, hexByte, hexByte[]);
};
#endif