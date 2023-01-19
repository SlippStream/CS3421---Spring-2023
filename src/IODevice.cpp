#include "IODevice.h"

emulIODevice::emulIODevice()
{
    reset();
}

void emulIODevice::initialize(emulDataMemory *dmem)
{
    dmemory = dmem;
}

void emulIODevice::reset()
{
    *reg = 0;
}

void emulIODevice::dump()
{
    std::cout << "IO Device: 0x" << std::setw(2) << std::setfill('0')
              << std::hex << (int)*reg << "\n\n";
}

void emulIODevice::load(std::string filename)
{
    filestream->open(filename);
    initialized = true;
    std::string temp;
    if (!filestream->eof())
    {
        std::getline(*filestream, temp);
        ss << temp;
        ss >> temp;
        listenTick = std::stoi(temp);
    }
}

void emulIODevice::read(uint8_t address)
{
    done = false;
    dmemory->startFetch(address, 1, reg, &done);
}

void emulIODevice::write(uint8_t address, uint8_t value)
{
    done = false;
    writeVal = value;
    dmemory->startStore(address, 1, &writeVal, &done);
}

void emulIODevice::startTick()
{
}

void emulIODevice::doCycleWork()
{
    if (!initialized)
        return;

    if (listenTick == ticks)
    {
        std::string temp;
        ss >> temp;
        if (temp == "read")
        {
            ss >> temp;
            read(std::stoul(temp, nullptr, 16) & 0xFF);
            return;
        }
        if (temp == "write")
        {
            std::string t2;
            ss >> temp;
            ss >> t2;
            write(std::stoul(temp, nullptr, 16) & 0xFF, std::stoul(t2, nullptr, 16) & 0xFF);
            return;
        }

        if (!filestream->eof())
        {
            std::getline(*filestream, temp);
            ss.clear();
            ss << temp;
            ss >> temp;
            listenTick = std::stoi(temp);
        }
    }
    ticks++;
}

bool emulIODevice::isMoreCycleWorkNeeded()
{
    return false;
}