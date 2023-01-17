#include "imemory.h"

emulInstrMemory::emulInstrMemory()
{
}

void emulInstrMemory::create(unsigned int size)
{
    sz = size;
    words = new wordContainer(size);
}

void emulInstrMemory::reset()
{
    for (unsigned int i = 0; i < sz; i++)
    {
        words->set(i, 0);
    }
}

void emulInstrMemory::dump(unsigned int address, unsigned int count)
{
    std::cout << "Addr     0     1     2     3     4     5     6     7\n";
    unsigned int limit = address + count;
    for (unsigned int i = 0; i < limit; i = i + 8)
    {
        if (address > i + 8)
        {
            continue;
        }
        std::cout << "0x" << std::setw(2) << std::setfill('0')
                  << std::uppercase << std::hex << (int)i << ' ';
        for (unsigned int j = 0; j < 8; j++)
        {
            if (i + j < address)
            {
                std::cout << "      ";
                continue;
            }

            if (i + j > limit)
            {
                break;
            }

            std::cout << std::setw(5) << std::setfill('0') << std::uppercase
                      << std::hex << words->get(i + j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void emulInstrMemory::set(unsigned int address, wordContainer *newWords)
{
    // std::cout << "Setting @ " << address << std::endl;
    for (unsigned int i = 0; i < newWords->getSize(); i++)
    {
        // std::cout << "Setting word " << (int)i << " to be " << newWords->get(i) << std::endl;
        words->set(address + i, newWords->get(i));
    }
}

unsigned int emulInstrMemory::load(unsigned int address)
{
    return words->get(address);
}
