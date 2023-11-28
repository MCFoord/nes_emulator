#include "bus.h"

uint8_t Bus::read(uint16_t addr)
{
    if (addr > 0x000 && addr < 0xFFFF)
    {
        return ram[addr];
    }
}

void Bus::write(uint16_t addr, uint8_t value)
{
    if (addr > 0x000 && addr < 0xFFFF)
    {
        ram[addr] = value;
    }
}