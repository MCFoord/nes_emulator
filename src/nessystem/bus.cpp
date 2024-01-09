#include <sstream>
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
    for (uint16_t i = start; i <= end; ++i)
    {
        if (count % 30 == 0 && count != 0)
        {
            ss << "\n";
        }
        ss << std::setfill('0') << std::setw(2) << std::hex << std::to_string(ram[i]) << " ";
        ++count;
    }

    return ss.str();
}
