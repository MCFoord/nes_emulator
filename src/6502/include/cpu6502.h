#ifndef CPU6502_
#define CPU6502_

#include <stdint.h>
#include <unordered_map>

class Bus;

class CPU6502
{
    public:
        CPU6502();
        ~CPU6502();

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
        uint8_t cycles = 0;

        

        enum CPUFLAGS
        {
            C = (1 << 0),
            Z = (1 << 1),
            I = (1 << 2),
            D = (1 << 3),
            B = (1 << 4),
            U = (1 << 5),
            V = (1 << 6),
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

        //instructions
        void ADC();
        void AND();
        void ASL();
        void BCC();
        void BCS();
        void BEQ();
        void BIT();
        void BMI();
        void BNE();
        void BPL();
        void BRK();
        void BVC();
        void BVS();
        void CLC();
        void CLD();
        void CLI();
        void CLV();
        void CMP();
        void CPX();
        void CPY();
        void DEC();
        void DEX();
        void DEY();
        void EOR();
        void INC();
        void INX();
        void INY();
        void JMP();
        void JSR();
        void LDA();
        void LDX();
        void LDY();
        void LSR();
        void NOP();
        void ORA();
        void PHA();
        void PHP();
        void PLA();
        void PLP();
        void ROL();
        void ROR();
        void RTI();
        void RTS();
        void SBC();
        void SEC();
        void SED();
        void SEI();
        void STA();
        void STX();
        void STY();
        void TAX();
        void TAY();
        void TSX();
        void TXA();
        void TXS();
        void TYA();

        struct instruction
        {
            uint8_t opcode;
            void (CPU6502::*addressingMode)();
            void (CPU6502::*operation)();
            uint8_t cycles;
        };

        std::unordered_map<uint8_t, instruction> instructions
        {
            {0x00, {0x00, implicit, BRK, 7}},
            {0x01, {0x01, indirectX, ORA, 6}},
            {0x05, {0x05, zeroPage, ORA, 3}},
            {0x06, {0x06, zeroPage, ASL, 5}},
            {0x08, {0x08, implicit, PHP, 3}},
            {0x09, {0x09, immediate, ORA, 2}},
            {0x0A, {0x0A, implicit, ASL, 2}},
            {0x0D, {0x0D, absolute, ORA, 4}},
            {0x0E, {0x0E, absolute, ASL, 6}},

            {0x10, {0x10, relative, BPL, 2}},
            {0x11, {0x11, indirectY, ORA, 5}},
            {0x15, {0x15, zeroPage, ORA, 4}},
            {0x16, {0x16, zeroPage, ASL, 6}},
            {0x18, {0x18, implicit, CLC, 2}},
            {0x19, {0x19, absoluteY, ORA, 4}},
            {0x1D, {0x1D, absoluteX, ORA, 4}},
            {0x1E, {0x1E, absoluteX, ASL, 7}},

            {0x20, {0x20, absolute, JSR, 6}},
            {0x21, {0x21, indirectX, AND, 6}},
            {0x24, {0x24, zeroPage, BIT, 3}},
            {0x25, {0x25, zeroPage, AND, 3}},
            {0x26, {0x26, zeroPage, ROL, 5}},
            {0x28, {0x28, implicit, PLP, 4}},
            {0x29, {0x29, immediate, AND, 2}},
            {0x2A, {0x2A, implicit, ROL, 2}},
            {0x2C, {0x2C, absolute, BIT, 4}},
            {0x2D, {0x2D, absolute, AND, 4}},
            {0x2E, {0x2E, absolute, ROL, 6}},

            {0x30, {0x30, relative, BMI, 2}},
            {0x31, {0x31, indirectY, AND, 5}},
            {0x35, {0x35, zeroPageX, AND, 4}},
            {0x36, {0x36, zeroPageX, ROL, 6}},
            {0x38, {0x38, implicit, SEC, 2}},
            {0x39, {0x39, absoluteY, AND, 4}},
            {0x3D, {0x3D, absoluteX, AND, 4}},
            {0x3E, {0x3E, absoluteX, ROL, 7}},

            {0x40, {0x40, , , }},
            {0x41, {0x41, , , }},
            {0x45, {0x45, , , }},
            {0x46, {0x46, , , }},
            {0x48, {0x48, , , }},
            {0x49, {0x49, , , }},
            {0x4A, {0x4A, , , }},
            {0x4C, {0x4C, , , }},
            {0x4D, {0x4D, , , }},
            {0x4E, {0x4E, , , }},

            {0x50, {0x50, , , }},
            {0x51, {0x51, , , }},
            {0x55, {0x55, , , }},
            {0x56, {0x56, , , }},
            {0x58, {0x58, , , }},
            {0x59, {0x59, , , }},
            {0x5D, {0x5D, , , }},
            {0x5E, {0x5E, , , }},

            {0x60, {0x60, , , }},
            {0x61, {0x61, , , }},
            {0x65, {0x65, , , }},
            {0x66, {0x66, , , }},
            {0x68, {0x68, , , }},
            {0x69, {0x69, , , }},
            {0x6A, {0x6A, , , }},
            {0x6C, {0x6C, , , }},
            {0x6D, {0x6D, , , }},
            {0x6E, {0x6E, , , }},

            {0x70, {0x70, , , }},
            {0x71, {0x71, , , }},
            {0x75, {0x75, , , }},
            {0x76, {0x76, , , }},
            {0x78, {0x78, , , }},
            {0x79, {0x79, , , }},
            {0x7D, {0x7D, , , }},
            {0x7E, {0x7E, , , }},

            {0x81, {0x81, , , }},
            {0x84, {0x84, , , }},
            {0x85, {0x85, , , }},
            {0x86, {0x86, , , }},
            {0x88, {0x88, , , }},
            {0x8A, {0x8A, , , }},
            {0x8C, {0x8C, , , }},
            {0x8D, {0x8D, , , }},
            {0x8E, {0x8E, , , }},

            {0x90, {0x90, , , }},
            {0x91, {0x91, , , }},
            {0x94, {0x94, , , }},
            {0x95, {0x95, , , }},
            {0x96, {0x96, , , }},
            {0x98, {0x98, , , }},
            {0x99, {0x99, , , }},
            {0x9A, {0x9A, , , }},
            {0x9D, {0x9D, , , }},

            {0xA0, {0xA0, , , }},
            {0xA1, {0xA1, , , }},
            {0xA2, {0xA2, , , }},
            {0xA4, {0xA4, , , }},
            {0xA5, {0xA5, , , }},
            {0xA6, {0xA6, , , }},
            {0xA8, {0xA8, , , }},
            {0xA9, {0xA9, , , }},
            {0xAA, {0xAA, , , }},
            {0xAC, {0xAC, , , }},
            {0xAD, {0xAD, , , }},
            {0xAE, {0xAE, , , }},

            {0xB0, {0xB0, , , }},
            {0xB1, {0xB1, , , }},
            {0xB4, {0xB4, , , }},
            {0xB5, {0xB5, , , }},
            {0xB6, {0xB6, , , }},
            {0xB8, {0xB8, , , }},
            {0xB9, {0xB9, , , }},
            {0xBA, {0xBA, , , }},
            {0xBC, {0xBC, , , }},
            {0xBD, {0xBD, , , }},
            {0xBE, {0xBE, , , }},

            {0xC0, {0xC0, , , }},
            {0xC1, {0xC1, , , }},
            {0xC4, {0xC4, , , }},
            {0xC5, {0xC5, , , }},
            {0xC6, {0xC6, , , }},
            {0xC8, {0xC8, , , }},
            {0xC9, {0xC9, , , }},
            {0xCA, {0xCA, , , }},
            {0xCC, {0xCC, , , }},
            {0xCD, {0xCD, , , }},
            {0xCE, {0xCE, , , }},

            {0xD0, {0xD0, , , }},
            {0xD1, {0xD1, , , }},
            {0xD5, {0xD5, , , }},
            {0xD6, {0xD6, , , }},
            {0xD8, {0xD8, , , }},
            {0xD9, {0xD9, , , }},
            {0xDD, {0xDD, , , }},
            {0xDE, {0xDE, , , }},

            {0xC0, {0xC0, , , }},
            {0xC1, {0xC1, , , }},
            {0xC4, {0xC4, , , }},
            {0xC5, {0xC5, , , }},
            {0xC6, {0xC6, , , }},
            {0xC8, {0xC8, , , }},
            {0xC9, {0xC9, , , }},
            {0xCA, {0xCA, , , }},
            {0xCC, {0xCC, , , }},
            {0xCD, {0xCD, , , }},
            {0xCE, {0xCE, , , }},

            {0xF0, {0xF0, , , }},
            {0xF1, {0xF1, , , }},
            {0xF5, {0xF5, , , }},
            {0xF6, {0xF6, , , }},
            {0xF8, {0xF8, , , }},
            {0xF9, {0xF9, , , }},
            {0xFD, {0xFD, , , }},
            {0xFE, {0xFE, , , }},
        };

        
};


#endif

