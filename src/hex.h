#ifndef HEX_H
#define HEX_H

class hexByte
{
private:
    char value;

public:
    hexByte(int);

    hexByte(char);

    hexByte();

    char *toString();

    int toInt();

    char toChar();

    hexByte operator+(hexByte const &);

    hexByte operator-(hexByte const &);
};

#endif