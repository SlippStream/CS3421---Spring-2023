#include "memory.h"

emulMemory::emulMemory()
{
    sz = 0;
}

void emulMemory::create(int size)
{
    bytes = new uint8_t[size];
    sz = size;
    reset();
}

void emulMemory::reset()
{
    for (int i = 0; i < sz; i++)
    {
        bytes[i] = 0x0;
    }
}

void emulMemory::dump(uint8_t address, uint8_t count)
{
    std::cout << "Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" << std::endl;
    unsigned int limit = address + count;
    for (unsigned int i = 0; i < limit; i = i + 16)
    {
        std::cout << "0x" << std::setw(2) << std::setfill('0')
                  << std::uppercase << std::hex << (int)i << ' ';
        for (unsigned int j = 0; j < 16; j++)
        {
            if (i + j < address)
            {
                std::cout << "   ";
                continue;
            }

            if (i + j > limit)
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

void emulMemory::set(uint8_t address, uint8_t count, std::string bytez[], int startAt)
{
    for (uint8_t i = 0; i < count; i++)
    {
        bytes[i + address] = std::stoul(bytez[i + startAt], nullptr, 16);
    }
}

uint8_t emulMemory::fetch(uint8_t address)
{

    if (address > sizeof(bytes))
    {
        throw 0;
    }
    return bytes[address];
}