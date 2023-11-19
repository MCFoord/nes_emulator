#ifndef CPU6502_
#define CPU6502_

#include <stdint.h>
#include <vector>

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

        instruction ins = {0x00, &CPU6502::implicit, &CPU6502::BRK, 7};


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

        void push(uint8_t value);
        uint8_t pop();

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

        std::vector<instruction> instructions;
};


#endif

