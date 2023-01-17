#include "hex_words.h"

wordContainer::wordContainer(unsigned int sz)
{
    size = sz;
    words = new wordPair[(sz / 2) + (sz & 0x01)];
}

unsigned int wordContainer::get(unsigned int index)
{
    if (index > size - 1 || size == 0)
    {
        throw "Index out of bounds!";
    }
    return (*getPair(index))[(uint8_t)(index & 0x1)];
}

void wordContainer::set(unsigned int index, unsigned int newVal)
{
    getPair(index)->set((uint8_t)(index & 0x1), newVal);
}

unsigned int wordContainer::operator[](unsigned int index)
{
    return get(index);
}

unsigned int wordContainer::getSize()
{
    return size;
}

/**
 *
 * wordPair
 *
 */

wordPair::wordPair()
{
    bytes = new uint8_t[5];
}

wordPair *wordContainer::getPair(unsigned int index)
{
    return words + (index / 2);
}

unsigned int wordPair::get(uint8_t index)
{
    validateIndex(index);

    unsigned int res = 0;
    uint8_t *ptr = bytes;

    // If it's even
    if (index == 0)
    {
        res += ((int)(*ptr)) << 12;
        ptr++;
        res += ((int)(*ptr)) << 4;
        ptr++;
        res += (int)(((*ptr) & 0xF0) >> 4);
        return res;
    }
    // If it's odd
    ptr += 2;
    res += ((int)(*ptr) & 0x0F) << 16;
    ptr++;
    res += ((int)(*ptr)) << 8;
    ptr++;
    res += (int)((*ptr));
    return res;
}

void wordPair::validateIndex(uint8_t index)
{
    if (index > 1)
    {
        throw "Index out of bounds";
    }
}

void wordPair::set(uint8_t index, unsigned int newVal)
{
    validateIndex(index);

    uint8_t *ptr = bytes;
    // std::cout << "Setting index " << (int)index << " to " << newVal << std::endl;

    // If it's even
    if (index == 0)
    {
        *ptr = (newVal & 0xFF000) >> 12;
        ptr++;
        *ptr = (newVal & 0x00FF0) >> 4;
        ptr++;
        *ptr = *ptr & 0x0F;
        *ptr = *ptr + ((newVal & 0x0000F) << 4);
        return;
    }
    // If it's odd
    ptr += 2;
    *ptr = ((newVal & 0xF0000) >> 16) + (*ptr & 0xF0);
    ptr++;
    *ptr = (newVal & 0x0FF00) >> 8;
    ptr++;
    *ptr = (newVal & 0x000FF);
    return;
}

unsigned int wordPair::operator[](uint8_t index)
{
    return get(index);
}