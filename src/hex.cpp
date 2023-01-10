#include <hex.h>

hexByte::hexByte(int v)
{
    value = (char)(v % 255);
}

hexByte::hexByte(char v)
{
    value = v;
}

hexByte::hexByte()
{
    value = 0;
}

char *hexByte::toString()
{
    char temp = value % 16;
    char res[5] = {'0',
                   'x',
                   value >> 4,
                   temp,
                   '\0'};
}

int hexByte::toInt()
{
    return (int)value;
}

char hexByte::toChar()
{
    return value;
}

hexByte hexByte::operator+(hexByte const &obj)
{
    char c = value + obj.value;
    hexByte res;
    res.value = c;
    return res;
}

hexByte hexByte::operator-(hexByte const &obj)
{
    char c = value - obj.value;
    hexByte res;
    res.value = c;
    return res;
}