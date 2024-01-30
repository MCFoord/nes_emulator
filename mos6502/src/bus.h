#pragma once

#include <stdint.h>
#include <string>

class Bus
{
    public:
        Bus();
        uint8_t ram[64 * 1024];

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);

        //return string of the values in memory from start address to end address inclusive
        std::string memToString(uint16_t start, uint16_t end);

        void loadProgram(char* fileName);
};
