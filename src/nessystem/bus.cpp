#include <sstream>
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
    for (uint8_t i = start; i <= end; ++i)
    {
        ss << std::setfill('0') << std::setw(2) << std::hex << ram[i];
    }

    return ss.str();
}