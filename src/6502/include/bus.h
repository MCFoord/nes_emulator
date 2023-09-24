#ifndef BUS_
#define BUS_

#include <stdint.h>
#include "cpu6502.h"

class Bus
{
    public:
        CPU6502 cpu;
        uint8_t ram[64 * 1024];

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);
};


#endif