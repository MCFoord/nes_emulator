#ifndef CPU6502_
#define CPU6502_

#include <stdint.h>

class Bus;

class CPU6502
{
    public:

        //registers
        uint8_t a = 0x00;
        uint8_t x = 0x00;
        uint8_t y = 0x00;
        uint8_t sp = 0x00;
        uint16_t pc = 0x000;
        uint8_t status = 0x00;

        uint16_t currentAddress = 0x000;
        uint8_t currentValue = 0x00;
        uint8_t opcode = 0x00;

        

        enum CPUFLAGS
        {
            C = (1 << 0),
            Z = (1 << 1),
            I = (1 << 2),
            D = (1 << 3),
            B = (1 << 4),
            U = (1 << 5),
            O = (1 << 6),
            N = (1 << 7)
        };

        bool getFlag(CPUFLAGS flag);
        void setFlag(CPUFLAGS flag, bool set);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t value);



    private:
        Bus *bus = nullptr;
        void connectBus(Bus *b);

        void reset();

        //addressing modes
        void implicit();
        void immediate();
        void accumulator();
        void relative();

        void indirect();
        void indirectX();
        void indirectY();

        void absolute();
        void absoluteX();
        void absoluteY();

        void zeroPage();
        void zeroPageX();
        void zeroPageY();

        //


};


#endif