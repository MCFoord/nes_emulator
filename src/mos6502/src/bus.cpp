#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "bus.h"

uint8_t Bus::read(uint16_t addr)
{
    if (addr > 0x000 && addr <= 0xFFFF)
    {
        return ram[addr];
    }
}

void Bus::write(uint16_t addr, uint8_t value)
{
    if (addr > 0x000 && addr <= 0xFFFF)
    {
        ram[addr] = value;
    }
}

std::string Bus::memToString(uint16_t start, uint16_t end)
{

    std::stringstream ss;
    int count = 0;
    ss << "    ";
    for (int i = 0; i < 16; ++i)
    {
        ss << std::setfill('0') << std::setw(2) << std::hex << i << " ";
    }

    ss << "\n";

    for (uint16_t i = start; i <= end; ++i)
    {
        if (count % 16 == 0)
        {
            ss << "\n" << std::setfill('0') << std::setw(2) << std::hex << count / 16 << "  ";
        }
        ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(ram[i]) << " ";
        ++count;
    }

    return ss.str();
}

void Bus::loadProgram(char* fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);

    file.read(reinterpret_cast<char*>(ram), length);

}