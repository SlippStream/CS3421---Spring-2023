#include "emulator.h"

struct tagged_args
{
    devices device;
    commands command;
    std::string *args;
};

emulator::emulator()
{
    em_cpu->initialize(em_dmemory, em_imemory);
    clock_clients = std::vector<clockClient *>();

    // Add clock clients
    clock_clients.push_back(em_cpu);
    clock_clients.push_back(em_dmemory);
}

struct tagged_args *emulator::tokenize(std::string str)
{
    std::stringstream ss;
    ss << str;
    devices device;
    commands command;

    std::string temp;
    // Grab device
    while (ss >> temp)
    {
        // std::cout << "device " << temp << std::endl;
        if (temp == "memory")
        {
            device = DEV_DMEMORY;
            break;
        }
        if (temp == "cpu")
        {
            device = DEV_CPU;
            break;
        }
        if (temp == "imemory")
        {
            device = DEV_IMEMORY;
            break;
        }
        if (temp == "clock")
        {
            device = DEV_CLOCK;
            break;
        }
        if (temp == "cache")
        {
            device = DEV_CACHE;
            break;
        }
        std::cout << "Device " << temp << " not found!" << std::endl;
        return nullptr;
    }
    // Grab command
    while (ss >> temp)
    {
        if (temp == "create")
        {
            command = CMD_CREATE;
            break;
        }

        if (temp == "reset")
        {
            command = CMD_RESET;
            break;
        }

        if (temp == "dump")
        {
            command = CMD_DUMP;
            break;
        }

        if (temp == "set")
        {
            command = CMD_SET;
            break;
        }

        if (temp == "tick")
        {
            command = CMD_TICK;
            break;
        }

        if (temp == "on")
        {
            command = CMD_ON;
            break;
        }

        if (temp == "off")
        {
            command = CMD_OFF;
            break;
        }
    }
    // Grab arguments
    std::vector<std::string> vec;
    while (ss >> temp)
    {
        if (device == DEV_IMEMORY && command == CMD_SET && temp == "file")
        {
            continue;
        }
        if (device == DEV_CPU && command == CMD_SET && temp == "reg")
        {
            continue;
        }
        vec.push_back(temp);
    }

    std::string *args;
    if (vec.size() > 0)
    {
        //! WILL CAUSE SEGFAULT IF GIVEN NO ARGUMENTS
        args = new std::string[vec.size()];
        for (unsigned int i = 0; i < vec.size(); i++)
        {
            args[i] = vec.at(i);
        }
    }
    struct tagged_args *p = new tagged_args;
    p->device = device;
    p->command = command;
    p->args = args;
    return p;
}

void emulator::emulate(std::ifstream &stream)
{
    if (stream.is_open())
    {
        std::string buffer;
        while (!stream.eof())
        {
            std::getline(stream, buffer);
            tagged_args *tags = tokenize(buffer);

            passCommand(tags->device, tags->command, tags->args);
        }
        return;
    }
    std::cout << "File not open!";
}

void emulator::passCommand(devices device, commands command, std::string args[])
{
    switch (device)
    {
    case DEV_DMEMORY:
        switch (command)
        {
        case CMD_CREATE:
            em_dmemory->create((int)std::stoul(args[0], nullptr, 16));
            break;
        case CMD_RESET:
            em_dmemory->reset();
            break;
        case CMD_DUMP:
            em_dmemory->dump(std::stoul(args[0], nullptr, 16) & 0xff, std::stoul(args[1], nullptr, 16) & 0xff);
            break;
        case CMD_SET:
            em_dmemory->set(std::stoul(args[0], nullptr, 16) & 0xFF, std::stoul(args[1], nullptr, 16) & 0xFF, args, 2);
            break;
        default:
            std::cout << "Command not found for device DATA MEMORY!" << std::endl;
            break;
        }
        break;
    case DEV_IMEMORY:
        switch (command)
        {
        case CMD_CREATE:
            em_imemory->create(std::stoul(args[0], nullptr, 16) & 0xFFFFFFFF);
            break;
        case CMD_RESET:
            em_imemory->reset();
            break;
        case CMD_DUMP:
            em_imemory->dump(std::stoul(args[0], nullptr, 16) & 0xFFFFFFFF, std::stoul(args[1], nullptr, 16) & 0xFFFFFFFF);
            break;
        case CMD_SET:
        {
            std::ifstream file;
            file.open(args[1]);
            if (file.fail())
            {
                //! ERROR File fails to open
                std::cout << "Error opening file " << args[1] << "!" << std::endl;
                throw 2;
            }

            std::string line;
            std::stringstream ss;
            std::string temp;
            std::vector<std::string> vec;
            while (!file.eof())
            {
                std::getline(file, temp);
                vec.push_back(temp);
            }

            // std::cout << "vec size " << vec.size() << std::endl;
            wordContainer wc(vec.size());
            unsigned int i = 0;
            for (auto it = vec.begin(); it != vec.end(); it++)
            {
                wc.set(i, std::stoul(*it, nullptr, 16) & 0xFFFFF);
                i++;
            }
            em_imemory->set(std::stoul(args[0], nullptr, 16) & 0xFF, &wc);
            break;
        }
        default:
            std::cout << "Command not found for device IMEMORY!" << std::endl;
            break;
        }
        break;
    case DEV_CLOCK:
        switch (command)
        {
        case CMD_RESET:
            em_clock->reset();
            break;
        case CMD_TICK:
            em_clock->tick(std::stoi(args[0]), clock_clients);
            break;
        case CMD_DUMP:
            em_clock->dump();
            break;
        default:
            std::cout << "Command not found for device CLOCK!" << std::endl;
            break;
        }
        break;
    case DEV_CPU:
        switch (command)
        {
        case CMD_RESET:
            em_cpu->reset();
            break;
        case CMD_SET:
            em_cpu->setReg(std::stoi(args[1], nullptr, 16), std::stoul(args[2], nullptr, 16) & 0xFF);
            break;
        case CMD_DUMP:
            em_cpu->dump();
            break;
        default:
            std::cout << "Command not found for device CPU!" << std::endl;
            break;
        }
        break;
    case DEV_CACHE:
        switch (command)
        {
        case CMD_RESET:
            em_cache->reset();
            break;
        case CMD_ON:
            em_cache->on();
            break;
        case CMD_OFF:
            em_clock->off();
            break;
        case CMD_DUMP:
            em_clock->dump();
            break;
        default:
            std::cout << "Command not found for device CACHE!" << std::endl;
            break;
        }
        break;
    default:
        std::cout << "Device not found!" << std::endl;
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        //! ERROR too many arguments
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }
    std::ifstream file;
    file.open(argv[1]);

    if (file.fail())
    {
        //! ERROR File fails to open
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