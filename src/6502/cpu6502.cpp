#include <sstream>
#include <iomanip>
#include <iostream>
#include "cpu6502.h"
#include "bus.h"

#define ILLEGAL(opcode) {opcode, "ILL", "implicit", &CPU6502::implicit, &CPU6502::ILL, 2}

CPU6502::CPU6502()
{
    instructions = {
        {0x00, "BRK", "implicit", &CPU6502::implicit, &CPU6502::BRK, 7},
        {0x01, "ORA", "indirectX", &CPU6502::indirectX, &CPU6502::ORA, 6},
        ILLEGAL(0x02),
        ILLEGAL(0x03),
        ILLEGAL(0x04),
        {0x05, "ORA", "zeroPage", &CPU6502::zeroPage, &CPU6502::ORA, 3},
        {0x06, "ASL", "zeroPage", &CPU6502::zeroPage, &CPU6502::ASL, 5},
        ILLEGAL(0x07),
        {0x08, "PHP", "implicit", &CPU6502::implicit, &CPU6502::PHP, 3},
        {0x09, "ORA", "immediate", &CPU6502::immediate, &CPU6502::ORA, 2},
        {0x0A, "ASL", "accumulator", &CPU6502::accumulator, &CPU6502::ASL, 2},
        ILLEGAL(0x0B),
        ILLEGAL(0x0C),
        {0x0D, "ORA", "absolute", &CPU6502::absolute, &CPU6502::ORA, 4},
        {0x0E, "ASL", "absolute", &CPU6502::absolute, &CPU6502::ASL, 6},
        ILLEGAL(0x0F),

        {0x10, "BPL", "relative", &CPU6502::relative, &CPU6502::BPL, 2},
        {0x11, "ORA", "indirectY", &CPU6502::indirectY, &CPU6502::ORA, 5},
        ILLEGAL(0x12),
        ILLEGAL(0x13),
        ILLEGAL(0x14),
        {0x15, "ORA", "zeroPage", &CPU6502::zeroPage, &CPU6502::ORA, 4},
        {0x16, "ASL", "zeroPage", &CPU6502::zeroPage, &CPU6502::ASL, 6},
        ILLEGAL(0x17),
        {0x18, "CLC", "implicit", &CPU6502::implicit, &CPU6502::CLC, 2},
        {0x19, "ORA", "absoluteY", &CPU6502::absoluteY, &CPU6502::ORA, 4},
        ILLEGAL(0x1A),
        ILLEGAL(0x1B), 
        ILLEGAL(0x1C),
        {0x1D, "ORA", "absoluteX", &CPU6502::absoluteX, &CPU6502::ORA, 4},
        {0x1E, "ASL", "absoluteX", &CPU6502::absoluteX, &CPU6502::ASL, 7},
        ILLEGAL(0x1F),

        {0x20, "JSR", "absolute", &CPU6502::absolute, &CPU6502::JSR, 6},
        {0x21, "AND", "indirectX", &CPU6502::indirectX, &CPU6502::AND, 6},
        ILLEGAL(0x22),
        ILLEGAL(0x23),
        {0x24, "BIT", "zeroPage", &CPU6502::zeroPage, &CPU6502::BIT, 3},
        {0x25, "AND", "zeroPage", &CPU6502::zeroPage, &CPU6502::AND, 3},
        {0x26, "ROL", "zeroPage", &CPU6502::zeroPage, &CPU6502::ROL, 5},
        ILLEGAL(0x27),
        {0x28, "PLP", "implicit", &CPU6502::implicit, &CPU6502::PLP, 4},
        {0x29, "AND", "immediate", &CPU6502::immediate, &CPU6502::AND, 2},
        {0x2A, "ROL", "accumulator", &CPU6502::accumulator, &CPU6502::ROL, 2},
        ILLEGAL(0x2B),
        {0x2C, "BIT", "absolute", &CPU6502::absolute, &CPU6502::BIT, 4},
        {0x2D, "AND", "absolute", &CPU6502::absolute, &CPU6502::AND, 4},
        {0x2E, "ROL", "absolute", &CPU6502::absolute, &CPU6502::ROL, 6},
        ILLEGAL(0x2F),

        {0x30, "BMI", "relative", &CPU6502::relative, &CPU6502::BMI, 2},
        {0x31, "AND", "indirectY", &CPU6502::indirectY, &CPU6502::AND, 5},
        ILLEGAL(0x32),
        ILLEGAL(0x33),
        ILLEGAL(0x34),
        {0x35, "AND", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::AND, 4},
        {0x36, "ROL", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::ROL, 6},
        ILLEGAL(0x37),
        {0x38, "SEC", "implicit", &CPU6502::implicit, &CPU6502::SEC, 2},
        {0x39, "AND", "absoluteY", &CPU6502::absoluteY, &CPU6502::AND, 4},
        ILLEGAL(0x3A),
        ILLEGAL(0x3B),
        ILLEGAL(0x3C),
        {0x3D, "AND", "absoluteX", &CPU6502::absoluteX, &CPU6502::AND, 4},
        {0x3E, "ROL", "absoluteX", &CPU6502::absoluteX, &CPU6502::ROL, 7},
        ILLEGAL(0x3F),

        {0x40, "RTI", "implicit", &CPU6502::implicit, &CPU6502::RTI, 6},
        {0x41, "EOR", "indirectX", &CPU6502::indirectX, &CPU6502::EOR, 6},
        ILLEGAL(0x42),
        ILLEGAL(0x43),
        ILLEGAL(0x44),
        {0x45, "EOR", "zeroPage", &CPU6502::zeroPage, &CPU6502::EOR, 3},
        {0x46, "LSR", "zeroPage", &CPU6502::zeroPage, &CPU6502::LSR, 5},
        ILLEGAL(0x47),
        {0x48, "PHA", "implicit", &CPU6502::implicit, &CPU6502::PHA, 3},
        {0x49, "EOR", "immediate", &CPU6502::immediate, &CPU6502::EOR, 2},
        {0x4A, "LSR", "accumulator", &CPU6502::accumulator, &CPU6502::LSR, 2},
        ILLEGAL(0x4B),
        {0x4C, "JMP", "absolute", &CPU6502::absolute, &CPU6502::JMP, 3},
        {0x4D, "EOR", "absolute", &CPU6502::absolute, &CPU6502::EOR, 4},
        {0x4E, "LSR", "absoluteX", &CPU6502::absoluteX, &CPU6502::LSR, 6},
        ILLEGAL(0x4F),

        {0x50, "BVC", "relative", &CPU6502::relative, &CPU6502::BVC, 2},
        {0x51, "EOR", "indirectY", &CPU6502::indirectY, &CPU6502::EOR, 5},
        ILLEGAL(0x52),
        ILLEGAL(0x53),
        ILLEGAL(0x54),
        {0x55, "EOR", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::EOR, 4},
        {0x56, "LSR", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::LSR, 6},
        ILLEGAL(0x57),
        {0x58, "CLI", "implicit", &CPU6502::implicit, &CPU6502::CLI, 2},
        {0x59, "EOR", "absoluteY", &CPU6502::absoluteY, &CPU6502::EOR, 4},
        ILLEGAL(0x5A),
        ILLEGAL(0x5B),
        ILLEGAL(0x5C),
        {0x5D, "EOR", "absoluteX", &CPU6502::absoluteX, &CPU6502::EOR, 4},
        {0x5E, "LSR", "absoluteX", &CPU6502::absoluteX, &CPU6502::LSR, 7},
        ILLEGAL(0x5F),

        {0x60, "RTS", "implicit", &CPU6502::implicit, &CPU6502::RTS, 6},
        {0x61, "ADC", "indirectX", &CPU6502::indirectX, &CPU6502::ADC, 6},
        ILLEGAL(0x62),
        ILLEGAL(0x63),
        ILLEGAL(0x64),
        {0x65, "ADC", "zeroPage", &CPU6502::zeroPage, &CPU6502::ADC, 3},
        {0x66, "ROR", "zeroPage", &CPU6502::zeroPage, &CPU6502::ROR, 5},
        ILLEGAL(0x67),
        {0x68, "PLA", "implicit", &CPU6502::implicit, &CPU6502::PLA, 4},
        {0x69, "ADC", "immediate", &CPU6502::immediate, &CPU6502::ADC, 4},
        {0x6A, "ROR", "accumulator", &CPU6502::accumulator, &CPU6502::ROR, 2},
        ILLEGAL(0x6B),
        {0x6C, "JMP", "indirect", &CPU6502::indirect, &CPU6502::JMP, 5},
        {0x6D, "ADC", "absolute", &CPU6502::absolute, &CPU6502::ADC, 4},
        {0x6E, "ROR", "absolute", &CPU6502::absolute, &CPU6502::ROR, 6},
        ILLEGAL(0x6F),

        {0x70, "BVS", "relative", &CPU6502::relative, &CPU6502::BVS, 2},
        {0x71, "ADC", "indirectY", &CPU6502::indirectY, &CPU6502::ADC, 5},
        ILLEGAL(0x72),
        ILLEGAL(0x73),
        ILLEGAL(0x74),
        {0x75, "ADC", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::ADC, 4},
        {0x76, "ROR", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::ROR, 6},
        ILLEGAL(0x77),
        {0x78, "SEI", "implicit", &CPU6502::implicit, &CPU6502::SEI, 2},
        {0x79, "ADC", "absoluteY", &CPU6502::absoluteY, &CPU6502::ADC, },
        ILLEGAL(0x7A),
        ILLEGAL(0x7B),
        ILLEGAL(0x7C),
        {0x7D, "ADC", "absoluteX", &CPU6502::absoluteX, &CPU6502::ADC, 4},
        {0x7E, "ROR", "absoluteX", &CPU6502::absoluteX, &CPU6502::ROR, 7},
        ILLEGAL(0x7F),

        ILLEGAL(0x80),
        {0x81, "STA", "indirectX", &CPU6502::indirectX, &CPU6502::STA, 6},
        ILLEGAL(0x82),
        ILLEGAL(0x83),
        {0x84, "STY", "zeroPage", &CPU6502::zeroPage, &CPU6502::STY, 3},
        {0x85, "STA", "zeroPage", &CPU6502::zeroPage, &CPU6502::STA, 3},
        {0x86, "STX", "zeroPage", &CPU6502::zeroPage, &CPU6502::STX, 3},
        ILLEGAL(0x87),
        {0x88, "DEY", "implicit", &CPU6502::implicit, &CPU6502::DEY, 2},
        ILLEGAL(0x89),
        {0x8A, "TXA", "implicit", &CPU6502::implicit, &CPU6502::TXA, 2},
        ILLEGAL(0x8B),
        {0x8C, "STY", "absolute", &CPU6502::absolute, &CPU6502::STY, 4},
        {0x8D, "STA", "absolute", &CPU6502::absolute, &CPU6502::STA, 4},
        {0x8E, "STX", "absolute", &CPU6502::absolute, &CPU6502::STX, 4},
        ILLEGAL(0x8F),

        {0x90, "BCC", "relative", &CPU6502::relative, &CPU6502::BCC, 2},
        {0x91, "STA", "indirectY", &CPU6502::indirectY, &CPU6502::STA, 6},
        ILLEGAL(0x92),
        ILLEGAL(0x93),
        {0x94, "STY", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::STY, 4},
        {0x95, "STA", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::STA, 4},
        {0x96, "STX", "zeroPageY", &CPU6502::zeroPageY, &CPU6502::STX, 4},
        ILLEGAL(0x97),
        {0x98, "TYA", "implicit", &CPU6502::implicit, &CPU6502::TYA, 2},
        {0x99, "STA", "absoluteY", &CPU6502::absoluteY, &CPU6502::STA, 5},
        {0x9A, "TXS", "implicit", &CPU6502::implicit, &CPU6502::TXS, 2},
        ILLEGAL(0x9B),
        ILLEGAL(0x9C),
        {0x9D, "STA", "absoluteX", &CPU6502::absoluteX, &CPU6502::STA, 5},
        ILLEGAL(0x9E),
        ILLEGAL(0x9F),

        {0xA0, "LDY", "immediate", &CPU6502::immediate, &CPU6502::LDY, 2},
        {0xA1, "LDA", "indirectX", &CPU6502::indirectX, &CPU6502::LDA, 6},
        {0xA2, "LDX", "immediate", &CPU6502::immediate, &CPU6502::LDX, 2},
        ILLEGAL(0xA3),
        {0xA4, "LDA", "zeroPage", &CPU6502::zeroPage, &CPU6502::LDA, 4},
        {0xA5, "LDA", "zeroPage", &CPU6502::zeroPage, &CPU6502::LDA, 4},
        {0xA6, "LDX", "zeroPage", &CPU6502::zeroPage, &CPU6502::LDX, 4},
        ILLEGAL(0xA7),
        {0xA8, "TAY", "implicit", &CPU6502::implicit, &CPU6502::TAY, 2},
        {0xA9, "LDA", "immediate", &CPU6502::immediate, &CPU6502::LDA, 2},
        {0xAA, "TAX", "implicit", &CPU6502::implicit, &CPU6502::TAX, 2},
        ILLEGAL(0xAB),
        {0xAC, "LDA", "absolute", &CPU6502::absolute, &CPU6502::LDA, 4},
        {0xAD, "LDA", "absolute", &CPU6502::absolute, &CPU6502::LDA, 4},
        {0xAE, "LDX", "absolute", &CPU6502::absolute, &CPU6502::LDX, 4},
        ILLEGAL(0xAF),

        {0xB0, "BCS", "relative", &CPU6502::relative, &CPU6502::BCS, 2},
        {0xB1, "LDA", "indirectY", &CPU6502::indirectY, &CPU6502::LDA, 5},
        ILLEGAL(0xB2),
        ILLEGAL(0xB3),
        {0xB4, "LDY", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::LDY, 4},
        {0xB5, "LDA", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::LDA, 4},
        {0xB6, "LDX", "zeroPageY", &CPU6502::zeroPageY, &CPU6502::LDX, 4},
        ILLEGAL(0xB7),
        {0xB8, "CLV", "implicit", &CPU6502::implicit, &CPU6502::CLV, 2},
        {0xB9, "LDA", "absoluteY", &CPU6502::absoluteY, &CPU6502::LDA, 4},
        {0xBA, "TSX", "implicit", &CPU6502::implicit, &CPU6502::TSX, 2},
        ILLEGAL(0xBB),
        {0xBC, "LDY", "absoluteX", &CPU6502::absoluteX, &CPU6502::LDY, 4},
        {0xBD, "LDA", "absoluteX", &CPU6502::absoluteX, &CPU6502::LDA, 4},
        {0xBE, "LDX", "absoluteY", &CPU6502::absoluteY, &CPU6502::LDX, 4},
        ILLEGAL(0xBF),

        {0xC0, "CPY", "immediate", &CPU6502::immediate, &CPU6502::CPY, 2},
        {0xC1, "CMP", "indirectX", &CPU6502::indirectX, &CPU6502::CMP, 6},
        ILLEGAL(0xC2),
        ILLEGAL(0xC3),
        {0xC4, "CPY", "zeroPage", &CPU6502::zeroPage, &CPU6502::CPY, 3},
        {0xC5, "CMP", "zeroPage", &CPU6502::zeroPage, &CPU6502::CMP, 3},
        {0xC6, "DEC", "zeroPage", &CPU6502::zeroPage, &CPU6502::DEC, 5},
        ILLEGAL(0xC7),
        {0xC8, "INY", "implicit", &CPU6502::implicit, &CPU6502::INY, 2},
        {0xC9, "CMP", "immediate", &CPU6502::immediate, &CPU6502::CMP, 2},
        {0xCA, "DEX", "implicit", &CPU6502::implicit, &CPU6502::DEX, 2},
        ILLEGAL(0xCB),
        {0xCC, "CPY", "absolute", &CPU6502::absolute, &CPU6502::CPY, 4},
        {0xCD, "CMP", "absolute", &CPU6502::absolute, &CPU6502::CMP, 4},
        {0xCE, "DEC", "absolute", &CPU6502::absolute, &CPU6502::DEC, 6},
        ILLEGAL(0xCF),

        {0xD0, "BNE", "relative", &CPU6502::relative, &CPU6502::BNE, 2},
        {0xD1, "CMP", "indirectY", &CPU6502::indirectY, &CPU6502::CMP, 5},
        ILLEGAL(0xD2),
        ILLEGAL(0xD3),
        ILLEGAL(0xD4),
        {0xD5, "CMP", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::CMP, 4},
        {0xD6, "DEC", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::DEC, 6},
        ILLEGAL(0xD7),
        {0xD8, "CLD", "implicit", &CPU6502::implicit, &CPU6502::CLD, 2 },
        {0xD9, "CMP", "absoluteY", &CPU6502::absoluteY, &CPU6502::CMP, 4},
        ILLEGAL(0xDA),
        ILLEGAL(0xDB),
        ILLEGAL(0xDC),
        {0xDD, "CMP", "absoluteX", &CPU6502::absoluteX, &CPU6502::CMP, 4},
        {0xDE, "DEC", "absoluteX", &CPU6502::absoluteX, &CPU6502::DEC, 7},
        ILLEGAL(0xDF),

        {0xE0, "CPX", "immediate", &CPU6502::immediate, &CPU6502::CPX, 2},
        {0xE1, "SBC", "indirectX", &CPU6502::indirectX, &CPU6502::SBC, 6},
        ILLEGAL(0xE2),
        ILLEGAL(0xE3),
        {0xE4, "CPX", "zeroPage", &CPU6502::zeroPage, &CPU6502::CPX, 3},
        {0xE5, "SBC", "zeroPage", &CPU6502::zeroPage, &CPU6502::SBC, 3},
        {0xE6, "INC", "zeroPage", &CPU6502::zeroPage, &CPU6502::INC, 5},
        ILLEGAL(0xE7),
        {0xE8, "INX", "implicit", &CPU6502::implicit, &CPU6502::INX, 2},
        {0xE9, "SBC", "immediate", &CPU6502::immediate, &CPU6502::SBC, 2},
        {0xEA, "NOP", "implicit", &CPU6502::implicit, &CPU6502::NOP, 2},
        ILLEGAL(0xEB),
        {0xEC, "CPX", "absolute", &CPU6502::absolute, &CPU6502::CPX, 4},
        {0xED, "SBC", "absolute", &CPU6502::absolute, &CPU6502::SBC, 4},
        {0xEE, "INC", "absolute", &CPU6502::absolute, &CPU6502::INC, 6},
        ILLEGAL(0xEF),

        {0xF0, "BEQ", "relative", &CPU6502::relative, &CPU6502::BEQ, 2},
        {0xF1, "SBC", "indirectY", &CPU6502::indirectY, &CPU6502::SBC, 5},
        ILLEGAL(0xF2),
        ILLEGAL(0xF3),
        ILLEGAL(0xF4),
        {0xF5, "SBC", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::SBC, 4},
        {0xF6, "INC", "zeroPageX", &CPU6502::zeroPageX, &CPU6502::INC, 6},
        ILLEGAL(0xF7),
        {0xF8, "SED", "implicit", &CPU6502::implicit, &CPU6502::SED, 2},
        {0xF9, "SBC", "absoluteY", &CPU6502::absoluteY, &CPU6502::SBC, 4},
        ILLEGAL(0xFA),
        ILLEGAL(0xFB),
        ILLEGAL(0xFC),
        {0xFD, "SBC", "absoluteX", &CPU6502::absoluteX, &CPU6502::SBC, 4},
        {0xFE, "INC", "absoluteX", &CPU6502::absoluteX, &CPU6502::INC, 7},
        ILLEGAL(0xFF)
    };
}

CPU6502::~CPU6502()
{
    
}

std::string CPU6502::registerToString()
{
    std::stringstream ss;
    ss << "A: " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(a) << ", "
       << "X: " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(x) << ", "
       << "Y: " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(y) << ", "
       << "SP: " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(sp) << ", "
       << "PC: " << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(pc);
      
    return ss.str();
}

std::string CPU6502::instructionInfoToString()
{
    std::stringstream ss;

    ss << "PC value: (" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(read(pc)) << ") "
       << ", Next Instruction: " << currentInstruction.instructionName << ", Adressing Mode: " << currentInstruction.addressingModeName << "\n"
       << "Previous operation: {address: " << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(currentAddress)
       << ", Value: " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(currentValue) << "}\n";

    return ss.str();
}

std::string CPU6502::statusToString()
{
    std::stringstream ss;
    ss << "C: " << getFlag(CPU6502::C) << ", "
       << "Z: " << getFlag(CPU6502::Z) << ", "
       << "I: " << getFlag(CPU6502::I) << ", "
       << "D: " << getFlag(CPU6502::D) << ", "
       << "B: " << getFlag(CPU6502::B) << ", "
       << "U: " << getFlag(CPU6502::U) << ", "
       << "V: " << getFlag(CPU6502::V) << ", "
       << "N: " << getFlag(CPU6502::N);

    return ss.str();
}

void CPU6502::fetch()
{
    currentInstruction = instructions[read(pc)];
}

uint8_t CPU6502::read(uint16_t addr)
{
    return bus->read(addr);
}

void CPU6502::write(uint16_t addr, uint8_t value)
{
    bus->write(addr, value);
}

void CPU6502::push(uint8_t value)
{
    write(0x0100 + sp, value);
    sp--;
}

uint8_t CPU6502::pop()
{
    sp++;
    return read(0x0100 + sp);
}

bool CPU6502::getFlag(CPUFLAGS flag)
{
    return flag & status;
}

void CPU6502::setFlag(CPUFLAGS flag, bool set)
{
    if (set)
    {
        status |= flag;
    }
    else
    {
        status &= ~(flag);
    }
}

void CPU6502::connectBus(Bus *b)
{
    bus = b;
}

void CPU6502::tick()
{

}

void CPU6502::reset()
{
    pc = 0x0400;
    a = 0x00;
    x = 0x00;
    y = 0x00;
    sp = 0xFD;
    status = 0x30;

    currentAddress = 0x000;
    currentValue = 0x00;
    currentInstruction = {};
}

void CPU6502::execute()
{
    currentInstruction = instructions[read(pc++)];
    
    (this->*currentInstruction.addressingMode)();
    (this->*currentInstruction.operation)();
    
    return;
}

void CPU6502::run(int numOperations)
{
    while (currentInstruction.instructionName != "ILL" && numOperations > 0)
    {
        execute();
        --numOperations;
    }
}

void CPU6502::run()
{
    while (currentInstruction.instructionName != "ILL")
    {
        execute();
    }
}

void CPU6502::run(std::ostream& output, int numOperations)
{
    while (currentInstruction.instructionName != "ILL" && numOperations > 0)
    {
        execute(output);
        --numOperations;
    }
}

void CPU6502::execute(std::ostream& output)
{
    currentInstruction = instructions[read(pc++)];
    uint16_t instructionAddress = pc;
    
    (this->*currentInstruction.addressingMode)();
    (this->*currentInstruction.operation)();

    printOperation(instructionAddress, output);
    return;
}

void CPU6502::printOperation(uint16_t address,  std::ostream& output)
{
    output << "[ " << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(address)
           << ": " << currentInstruction.instructionName << " " << std::setfill(' ') 
           << std::setw(11) << std::left << currentInstruction.addressingModeName << " ] "
           << registerToString() << " " << statusToString()
           << " ca: " << std::hex << static_cast<int>(currentAddress)
           << " cv: " << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(read(0x01fd)) << "\n";
}

// addressing
void CPU6502::implicit()
{
    return;
}

void CPU6502::immediate()
{
    currentValue = read(pc++);
    currentAddress = pc;
}

void CPU6502::accumulator()
{
    currentValue = a;
}

void CPU6502::relative()
{
    uint16_t offset = read(pc++);

    if (offset & 0x80)
    {
        offset |= 0xFF00;
    }

    currentValue = offset;
    currentAddress = pc + offset;
}

//look up the page boundary bug for JMP opcode
void CPU6502::indirect()
{
    uint16_t pointerLow = read(pc++);
    uint8_t pointerHigh = read(pc);

    currentAddress = (pointerHigh << 8) | pointerLow; //lowbyte pointer

    uint16_t lowByte = read(currentAddress);
    uint8_t highByte = read(currentAddress + 1);

    if (pointerLow == 0xFF)
    {
        //unsure if this is actually correct, but i'm tired
        highByte = read(((currentAddress & 0xFF00) << 8) | pointerLow);
    }

    currentAddress = (highByte << 8) | lowByte;
    currentValue = read(currentAddress);
}

void CPU6502::indirectX()
{
    currentAddress = pc++;
    uint16_t lowByte = read((read(pc) + x) & 0x00FF);
    uint16_t highByte = read(((read(pc) + x + 1) & 0x00FF));

    currentAddress = (highByte << 8) | lowByte;

    currentValue = read(currentAddress);
}

void CPU6502::indirectY()
{
    uint16_t lowByte = read(pc++);
    uint16_t highByte = read(pc++);

    currentAddress = ((highByte << 8) | lowByte) + y;
    
    if (currentAddress & 0xFF00 != highByte)
    {
        cycles++;
    }

    currentValue = read(currentAddress);
}

void CPU6502::absolute()
{
    uint16_t lowByte = read(pc++);
    currentAddress = (read(pc++) << 8) | lowByte;

    currentValue = read(currentAddress);
} 

void CPU6502::absoluteX()
{
	uint16_t addr;
	uint16_t addrL;
	uint16_t addrH;

	addrL = read(pc++);
	addrH = read(pc++);

	addr = addrL + (addrH << 8) + x;

    currentAddress = addr;
	
    currentValue = read(currentAddress);
}

void CPU6502::absoluteY()
{
    uint16_t lowByte = read(pc);
    uint16_t highByte = read(pc++);

    currentAddress = ((highByte << 8) | lowByte) + y;

    if (currentAddress & 0xFF00 != highByte)
    {
        cycles++;
    }

    currentValue = read(currentAddress);
}

/* check this later for any bugs to do with not actually
    assigning the value address to current address
*/
void CPU6502::zeroPage()
{
    currentAddress = read(pc++) & 0xFF;
    currentValue = read(currentAddress);
}   

void CPU6502::zeroPageX()
{
    currentAddress = (read(pc++) + x) & 0xFF;
    currentValue = read(currentAddress);
}

void CPU6502::zeroPageY()
{
    currentAddress = (read(pc++) + y) & 0xFF;
    currentValue = read(currentAddress);
}


//inctructions
void CPU6502::ADC()
{
    uint16_t result = a + currentValue + getFlag(CPUFLAGS::C);

    setFlag(CPUFLAGS::Z, result == 0x00);
    setFlag(CPUFLAGS::C, result > 0xFF);
    setFlag(CPUFLAGS::V, (a ^ result) & (currentValue ^ result) & 0x80);
    setFlag(CPUFLAGS::N, result & 0x80);

    a = result & 0xFF;
}

void CPU6502::AND()
{
    a &= currentValue;

    setFlag(CPUFLAGS::Z, a == 0x00);
    setFlag(CPUFLAGS::N, a & 0x80);

}

void CPU6502::ASL()
{
    /*
    need to worry about where the result is being writen to,
    need to differentiate between if it's the accumulator or a memory address
    */
    
    uint8_t result = (currentValue << 1);

    setFlag(CPUFLAGS::Z, result == 0x00);
    setFlag(CPUFLAGS::C, currentValue & 0x80);
    setFlag(CPUFLAGS::N, result & 0x80);


    if (currentInstruction.addressingMode == &CPU6502::accumulator)
    {
        a = result;
    }
    else
    {
        write(currentAddress, result);
    }
}

void CPU6502::BCC()
{
    if (!getFlag(CPUFLAGS::C))
    {
        cycles++;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::BCS()
{
    if (getFlag(CPUFLAGS::C))
    {
        cycles++;
        uint16_t rel = pc + currentValue;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::BEQ()
{
    if (getFlag(CPUFLAGS::Z))
    {
        cycles++;
        uint16_t rel = pc + currentValue;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::BIT()
{
    uint8_t result = a & currentValue;

    setFlag(CPUFLAGS::Z, !result);
    setFlag(CPUFLAGS::V, currentValue & 0x40);
    setFlag(CPUFLAGS::N, currentValue & 0x80);
}

void CPU6502::BMI()
{
    if (getFlag(CPUFLAGS::N))
    {
        cycles++;
        uint16_t rel = pc + currentValue;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::BNE()
{
    if (!getFlag(CPUFLAGS::Z))
    {
        cycles++;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::BPL()
{
    if (!getFlag(CPUFLAGS::N))
    {
        cycles++;
        uint16_t rel = pc + currentValue;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::BRK()
{
    setFlag(CPU6502::B, true);
    setFlag(CPU6502::U, true);

    pc++;
    push((pc >> 8) & 0xFF);
    push(pc & 0xFF);
    push(status);

    setFlag(CPU6502::I, true);

    pc = (read(0xFFFF) << 8) | read(0xFFFE);
}

void CPU6502::BVC()
{
    if (!getFlag(CPUFLAGS::V))
    {
        cycles++;
        uint16_t rel = pc + currentValue;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::BVS()
{
    if (getFlag(CPUFLAGS::V))
    {
        cycles++;
        uint16_t rel = pc + currentValue;

        if (currentAddress & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = currentAddress;
    }
}

void CPU6502::CLC()
{
    setFlag(CPU6502::C, false);
}

void CPU6502::CLD()
{
    setFlag(CPU6502::D, false);
}

void CPU6502::CLI()
{
    setFlag(CPU6502::I, false);
}

void CPU6502::CLV()
{
    setFlag(CPU6502::V, false);
}

void CPU6502::CMP()
{
    uint8_t result = a - currentValue;

    setFlag(CPU6502::C, (a >= currentValue));
    setFlag(CPU6502::Z, (a == currentValue));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::CPX()
{
    uint8_t result = x - currentValue;

    setFlag(CPU6502::C, (x >= currentValue));
    setFlag(CPU6502::Z, (x == currentValue));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::CPY()
{
    uint8_t result = y - currentValue;

    setFlag(CPU6502::C, (y >= currentValue));
    setFlag(CPU6502::Z, (y == currentValue));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::DEC()
{
    uint8_t result = currentValue - 1;

    write(currentAddress, result);
    setFlag(CPU6502::Z, (result == 0x00));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::DEX()
{
    x--;

    setFlag(CPU6502::Z, (x == 0x00));
    setFlag(CPU6502::N, (x & 0x80));
}

void CPU6502::DEY()
{
    y--;

    setFlag(CPU6502::Z, (y == 0x00));
    setFlag(CPU6502::N, (y & 0x80));
}

void CPU6502::EOR()
{
    a ^= currentValue;

    setFlag(CPU6502::Z, (a == 0x00));
    setFlag(CPU6502::N, (a & 0x80));
}

void CPU6502::INC()
{
    uint8_t result = currentValue + 1;

    write(currentAddress, result);
    setFlag(CPU6502::Z, (result == 0x00));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::INX()
{
    x++;

    setFlag(CPU6502::Z, (x == 0x00));
    setFlag(CPU6502::N, (x & 0x80));
}

void CPU6502::INY()
{
    y++;

    setFlag(CPU6502::Z, (y == 0x00));
    setFlag(CPU6502::N, (y & 0x80));
}

//go to indirect page boundary bug
void CPU6502::JMP()
{
    pc = currentAddress;
}

void CPU6502::JSR()
{
    pc--;
    push((pc >> 8) & 0xFF);
    push(pc & 0xFF);
    pc = currentAddress;
}

void CPU6502::LDA()
{
    a = currentValue;
    setFlag(CPU6502::Z, (a == 0x00));
    setFlag(CPU6502::N, (a & 0x80));
}

void CPU6502::LDX()
{
    x = currentValue;
    setFlag(CPU6502::Z, (x == 0x00));
    setFlag(CPU6502::N, (x & 0x80));
}

void CPU6502::LDY()
{
    y = currentValue;
    setFlag(CPU6502::Z, (y == 0x00));
    setFlag(CPU6502::N, (y & 0x80));
}

void CPU6502::LSR()
{
    if (currentInstruction.addressingMode == &CPU6502::accumulator)
    {
        setFlag(CPU6502::C, (a & 0x01));
        a = (a >> 1);
        setFlag(CPU6502::Z, (a == 0x00));
        setFlag(CPU6502::N, (a & 0x80));
    }
    else
    {
        uint8_t result = (currentValue >> 1);

        setFlag(CPU6502::C, (currentValue & 0x01));
        setFlag(CPU6502::Z, (result == 0x00));
        setFlag(CPU6502::N, (result & 0x80));
        write(currentAddress, result);
    }
}

void CPU6502::NOP()
{

}

void CPU6502::ORA()
{
    a |= currentValue;

    setFlag(CPU6502::Z, (a == 0x00));
    setFlag(CPU6502::N, (a & 0x80));
}

void CPU6502::PHA()
{
    push(a);
}

void CPU6502::PHP()
{
    setFlag(CPU6502::B, true);
    setFlag(CPU6502::U, true);
    push(status);
}

void CPU6502::PLA()
{
    a = pop();
    setFlag(CPU6502::Z, (a == 0x00));
    setFlag(CPU6502::N, (a & 0x80));
}

void CPU6502::PLP()
{
    status = pop();
}

void CPU6502::ROL()
{
    
    if (currentInstruction.addressingMode == &CPU6502::accumulator)
    {
        uint8_t result = (a << 1) | getFlag(CPU6502::C);

        setFlag(CPU6502::C, (a & 0x80));
        setFlag(CPU6502::Z, (result == 0x00));
        setFlag(CPU6502::N, (result & 0x80));
        a = result;
    }
    else
    {
        uint8_t result = (currentValue << 1) | getFlag(CPU6502::C);

        setFlag(CPU6502::C, (currentValue & 0x80));
        setFlag(CPU6502::Z, (result == 0x00));
        setFlag(CPU6502::N, (result & 0x80));
        write(currentAddress, result);
    }
}

void CPU6502::ROR()
{
    if (currentInstruction.addressingMode == &CPU6502::accumulator)
    {
        uint8_t result = (a >> 1) | (getFlag(CPU6502::C) << 7);

        setFlag(CPU6502::C, (a & 0x01));
        setFlag(CPU6502::Z, (result == 0x00));
        setFlag(CPU6502::N, (result & 0x80));
        a = result;
    }
    else
    {
        uint8_t result = (currentValue >> 1) | (getFlag(CPU6502::C) << 7);

        setFlag(CPU6502::C, (currentValue & 0x01));
        setFlag(CPU6502::Z, (result == 0x00));
        setFlag(CPU6502::N, (result & 0x80));
        write(currentAddress, result);
    }
}

void CPU6502::RTI()
{
    status = pop();
    uint8_t highByte = pop();
    uint8_t lowByte = pop();

    pc = (highByte << 8) | lowByte;
}

void CPU6502::RTS()
{
    uint16_t lowByte = pop();
    uint8_t highByte = pop();

    pc = (highByte << 8) | lowByte;
    pc++;
}

void CPU6502::SBC()
{
    uint16_t result = a + ~currentValue + getFlag(CPUFLAGS::C);

    setFlag(CPUFLAGS::Z, result == 0x00);
    setFlag(CPUFLAGS::C, result > 0xFF);
    setFlag(CPUFLAGS::V, (a ^ result) & (currentValue ^ result) & 0x80);
    setFlag(CPUFLAGS::N, result & 0x80);

    a = result & 0xFF;
}

void CPU6502::SEC()
{
    setFlag(CPUFLAGS::C, true);
}

void CPU6502::SED()
{
    setFlag(CPUFLAGS::D, true);
}

void CPU6502::SEI()
{
    setFlag(CPUFLAGS::I, true);
}

void CPU6502::STA()
{
    write(currentAddress, a);
}

void CPU6502::STX()
{
    write(currentAddress, x);
}

void CPU6502::STY()
{
    write(currentAddress, y);
}

void CPU6502::TAX()
{
    x = a;
    setFlag(CPU6502::Z, (x == 0x00));
    setFlag(CPU6502::N, (x & 0x80));
}

void CPU6502::TAY()
{
    y = a;
    setFlag(CPU6502::Z, (y == 0x00));
    setFlag(CPU6502::N, (y & 0x80));
}

void CPU6502::TSX()
{
    x = sp;
    setFlag(CPU6502::Z, (x == 0x00));
    setFlag(CPU6502::N, (x & 0x80));
}

void CPU6502::TXA()
{
    a = x;
    setFlag(CPU6502::Z, (a == 0x00));
    setFlag(CPU6502::N, (a & 0x80));
}

void CPU6502::TXS() 
{
    sp = x;
}

void CPU6502::TYA()
{
    a = y;
    setFlag(CPU6502::Z, (a == 0x00));
    setFlag(CPU6502::N, (a & 0x80));
}


void CPU6502::ILL()
{

}