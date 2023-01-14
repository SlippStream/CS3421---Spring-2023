#include "emulator.h"

const std::string MEM_DEVICE_NAME = "memory";
const std::string CPU_DEVICE_NAME = "cpu";
const std::string CLOCK_DEVICE_NAME = "clock";

enum devices
{
    dev_cpu = 0,
    /** end clock clients */
    dev_memory = 1,
    dev_clock = 2
};

emulator::emulator()
{
    em_cpu->initialize(em_memory);
    clock_clients = std::vector<clockClient *>();

    // Add clock clients
    clock_clients.push_back(em_cpu);
}

std::vector<std::string> emulator::tokenize(std::string str)
{
    std::vector<std::string> vec;
    std::stringstream ss;
    ss << str;

    std::string temp;
    while (ss >> temp)
    {
        // std::cout << temp << std::endl;
        vec.push_back(temp);
    }
    // std::cout << std::endl;
    return vec;
}

void emulator::emulate(std::ifstream &stream)
{
    if (stream.is_open())
    {
        std::string buffer;
        std::string device;
        std::string command;
        std::string token;
        while (!stream.eof())
        {
            std::getline(stream, buffer);
            std::vector<std::string> vec = tokenize(buffer);
            auto it = vec.begin();

            // WILL CAUSE SEGFAULT IF GIVEN NO ARGUMENTS
            device = *it;
            it++;
            command = *it;
            it++;
            std::string tokens[vec.size() - 2];
            for (int i = 0; it != vec.end(); it++)
            {
                tokens[i] = *it;
                // std::cout << tokens[i] << std::endl;
                i++;
            }

            passCommand(device, command, tokens);
        }
        return;
    }
    std::cout << "File not open!";
}

void emulator::passCommand(std::string device, std::string command, std::string args[])
{
#pragma region MEMORY_COMMANDS
    if (device == MEM_DEVICE_NAME)
    {
        if (command == "create")
        {
            em_memory->create((int)std::stoul(args[0], nullptr, 16));
            return;
        }
        if (command == "reset")
        {
            em_memory->reset();
            return;
        }
        if (command == "dump")
        {
            em_memory->dump(std::stoul(args[0], nullptr, 16) & 0xff, std::stoul(args[1], nullptr, 16) & 0xff);
            return;
        }
        if (command == "set")
        {
            em_memory->set(std::stoul(args[0], nullptr, 16) & 0xFF, std::stoul(args[1], nullptr, 16) & 0xFF, args, 2);
            return;
        }
        return;
    }
#pragma endregion MEMORY_COMMANDS
#pragma region CLOCK_COMMANDS
    if (device == CLOCK_DEVICE_NAME)
    {
        if (command == "reset")
        {
            em_clock->reset();
            return;
        }
        if (command == "tick")
        {
            int i = std::stoi(args[0]);
            em_clock->tick(i, clock_clients);
            return;
        }
        if (command == "dump")
        {
            em_clock->dump();
            return;
        }
        return;
    }
#pragma endregion CLOCK_COMMANDS
#pragma region CPU_COMMANDS
    if (device == CPU_DEVICE_NAME)
    {
        if (command == "reset")
        {
            em_cpu->reset();
            return;
        }
        if (command == "set" && args[0] == "reg")
        {
            em_cpu->setReg(std::stoi(args[1], nullptr, 16), std::stoul(args[2], nullptr, 16) & 0xFF);
            return;
        }
        if (command == "dump")
        {
            em_cpu->dump();
            return;
        }
        return;
    }
#pragma endregion CPU_COMMANDS
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        // ERROR too many arguments
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }
    std::ifstream file;
    file.open(argv[1]);

    if (file.fail())
    {
        // ERROR File fails to open
        std::cout << "Error opening file " << argv[1] << "!" << std::endl;
        return 2;
    }

    try
    {
        emulator em;
        em.emulate(file);
    }
    catch (int f)
    {
        file.close();
        return 1;
    }

    file.close();

    return 0;
}