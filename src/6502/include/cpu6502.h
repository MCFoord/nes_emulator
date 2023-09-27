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

        struct instruction
        {
            uint8_t opcode;
            void (CPU6502::*addressingMode)();
            void (CPU6502::*operation)();
            uint8_t cycles;
        };


        uint16_t currentAddress = 0x000;
        uint8_t currentValue = 0x00;
        instruction currentInstruction;
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

            {0x40, {0x40, implicit, RTI, 6}},
            {0x41, {0x41, indirectX, EOR, 6}},
            {0x45, {0x45, zeroPage, EOR, 3}},
            {0x46, {0x46, zeroPage, LSR, 5}},
            {0x48, {0x48, implicit, PHA, 3}},
            {0x49, {0x49, immediate, EOR, 2}},
            {0x4A, {0x4A, implicit, LSR, 2}},
            {0x4C, {0x4C, absolute, JMP, 3}},
            {0x4D, {0x4D, absolute, EOR, 4}},
            {0x4E, {0x4E, absoluteX, LSR, 6}},

            {0x50, {0x50, relative, BVC, 2}},
            {0x51, {0x51, indirectY, EOR, 5}},
            {0x55, {0x55, zeroPageX, EOR, 4}},
            {0x56, {0x56, zeroPageX, LSR, 6}},
            {0x58, {0x58, implicit, CLI, 2}},
            {0x59, {0x59, absoluteY, EOR, 4}},
            {0x5D, {0x5D, absoluteX, EOR, 4}},
            {0x5E, {0x5E, absoluteX, LSR, 7}},

            {0x60, {0x60, implicit, RTS, 6}},
            {0x61, {0x61, indirectX, ADC, 6}},
            {0x65, {0x65, zeroPage, ADC, 3}},
            {0x66, {0x66, zeroPage, ROR, 5}},
            {0x68, {0x68, implicit, PLA, 4}},
            {0x69, {0x69, immediate, ADC, 4}},
            {0x6A, {0x6A, implicit, ROR, 2}},
            {0x6C, {0x6C, indirect, JMP, 5}},
            {0x6D, {0x6D, absolute, ADC, 4}},
            {0x6E, {0x6E, absolute, ROR, 6}},

            {0x70, {0x70, relative, BVS, 2}},
            {0x71, {0x71, indirectY, ADC, 5}},
            {0x75, {0x75, zeroPageX, ADC, 4}},
            {0x76, {0x76, zeroPageX, ROR, 6}},
            {0x78, {0x78, implicit, SEI, 2}},
            {0x79, {0x79, absoluteY, ADC, }},
            {0x7D, {0x7D, absoluteX, ADC, 4}},
            {0x7E, {0x7E, absoluteX, ROR, 7}},

            {0x81, {0x81, indirectX, STA, 6}},
            {0x84, {0x84, zeroPage, STY, 3}},
            {0x85, {0x85, zeroPage, STA, 3}},
            {0x86, {0x86, zeroPage, STX, 3}},
            {0x88, {0x88, implicit, DEY, 2}},
            {0x8A, {0x8A, implicit, TXA, 2}},
            {0x8C, {0x8C, absolute, STY, 4}},
            {0x8D, {0x8D, absolute, STA, 4}},
            {0x8E, {0x8E, absolute, STX, 4}},

            {0x90, {0x90, relative, BCC, 2}},
            {0x91, {0x91, indirectY, STA, 6}},
            {0x94, {0x94, zeroPageX, STY, 4}},
            {0x95, {0x95, zeroPageX, STA, 4}},
            {0x96, {0x96, zeroPageY, STX, 4}},
            {0x98, {0x98, implicit, TYA, 2}},
            {0x99, {0x99, absoluteY, STA, 5}},
            {0x9A, {0x9A, implicit, TXS, 2}},
            {0x9D, {0x9D, absoluteX, STA, 5}},

            {0xA0, {0xA0, immediate, LDY, 2}},
            {0xA1, {0xA1, indirectX, LDA, 6}},
            {0xA2, {0xA2, immediate, LDX, 2}},
            {0xA4, {0xA4, zeroPage, LDA, 4}},
            {0xA5, {0xA5, zeroPage, LDA, 4}},
            {0xA6, {0xA6, zeroPage, LDX, 4}},
            {0xA8, {0xA8, implicit, TAY, 2}},
            {0xA9, {0xA9, immediate, LDA, 2}},
            {0xAA, {0xAA, implicit, TAX, 2}},
            {0xAC, {0xAC, absolute, LDA, 4}},
            {0xAD, {0xAD, absolute, LDA, 4}},
            {0xAE, {0xAE, absolute, LDX, 4}},

            {0xB0, {0xB0, relative, BCS, 2}},
            {0xB1, {0xB1, indirectY, LDA, 5}},
            {0xB4, {0xB4, zeroPageX, LDY, 4}},
            {0xB5, {0xB5, zeroPageX, LDA, 4}},
            {0xB6, {0xB6, zeroPageY, LDX, 4}},
            {0xB8, {0xB8, implicit, CLV, 2}},
            {0xB9, {0xB9, absoluteY, LDA, 4}},
            {0xBA, {0xBA, implicit, TSX, 2}},
            {0xBC, {0xBC, absoluteX, LDY, 4}},
            {0xBD, {0xBD, absoluteX, LDA, 4}},
            {0xBE, {0xBE, absoluteY, LDX, 4}},

            {0xC0, {0xC0, immediate, CPY, 2}},
            {0xC1, {0xC1, indirectX, CMP, 6}},
            {0xC4, {0xC4, zeroPage, CPY, 3}},
            {0xC5, {0xC5, zeroPage, CMP, 3}},
            {0xC6, {0xC6, zeroPage, DEC, 5}},
            {0xC8, {0xC8, implicit, INY, 2}},
            {0xC9, {0xC9, immediate, CMP, 2}},
            {0xCA, {0xCA, implicit, DEX, 2}},
            {0xCC, {0xCC, absolute, CPY, 4}},
            {0xCD, {0xCD, absolute, CMP, 4}},
            {0xCE, {0xCE, absolute, DEC, 6}},

            {0xD0, {0xD0, relative, BNE, 2}},
            {0xD1, {0xD1, indirectY, CMP, 5}},
            {0xD5, {0xD5, zeroPageX, CMP, 4}},
            {0xD6, {0xD6, zeroPageX, DEC, 6}},
            {0xD8, {0xD8, implicit, CLD, 2}},
            {0xD9, {0xD9, absoluteY, CMP, 4}},
            {0xDD, {0xDD, absoluteX, CMP, 4}},
            {0xDE, {0xDE, absoluteX, DEC, 7}},

            {0xE0, {0xE0, immediate, CPX, 2}},
            {0xE1, {0xE1, indirectX, SBC, 6}},
            {0xE4, {0xE4, zeroPage, CPX, 3}},
            {0xE5, {0xE5, zeroPage, SBC, 3}},
            {0xE6, {0xE6, zeroPage, INC, 5}},
            {0xE8, {0xE8, implicit, INX, 2}},
            {0xE9, {0xE9, immediate, SBC, 2}},
            {0xEA, {0xEA, implicit, NOP, 2}},
            {0xEC, {0xEC, absolute, CPX, 4}},
            {0xED, {0xED, absolute, SBC, 4}},
            {0xEE, {0xEE, absolute, INC, 6}},

            {0xF0, {0xF0, relative, BEQ, 2}},
            {0xF1, {0xF1, indirectY, SBC, 5}},
            {0xF5, {0xF5, zeroPageX, SBC, 4}},
            {0xF6, {0xF6, zeroPageX, INC, 6}},
            {0xF8, {0xF8, implicit, SED, 2}},
            {0xF9, {0xF9, absoluteY, SBC, 4}},
            {0xFD, {0xFD, absoluteX, SBC, 4}},
            {0xFE, {0xFE, absoluteX, INC, 7}},
        };  
};


#endif

