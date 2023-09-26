#include "cpu6502.h"
#include "bus.h"

CPU6502::CPU6502()
{

}

CPU6502::~CPU6502()
{
    
}

uint8_t CPU6502::read(uint16_t addr)
{
    bus->read(addr);
}

void CPU6502::write(uint16_t addr, uint8_t value)
{
    bus->write(addr, value);
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


// addressing
void CPU6502::implicit()
{
    return;
}

void CPU6502::immediate()
{
    currentAddress = pc++;
    currentValue = read(pc);
}

void CPU6502::accumulator()
{
    currentValue = a;
}

void CPU6502::relative()
{
    currentAddress = pc++;
    uint8_t offset = read(pc);
    if (offset & 0x80)
    {
        offset |= 0xFF00;
    }

    currentAddress += offset;
}

//look up the page boundary bug for JMP opcode
void CPU6502::indirect()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;

    currentAddress = (read(pc) << 8) | lowByte;
}

void CPU6502::indirectX()
{
    currentAddress = pc++;
    uint16_t lowByte = read((read(pc) + x) & 0x00FF);
    uint16_t highByte = read(((read(pc) + x + 1) & 0x00FF));

    currentAddress = (highByte << 8) | lowByte;

}

void CPU6502::indirectY()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t highByte = read(pc);

    currentAddress = ((highByte << 8) | lowByte) + y;
    
    if (currentAddress & 0xFF00 != highByte)
    {
        cycles++;
    }
}

void CPU6502::absolute()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t valueAddress = (read(pc) << 8) | lowByte;

    currentValue = read(valueAddress);
}

void CPU6502::absoluteX()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t highByte = read(pc);
    uint16_t valueAddress = ((highByte << 8) | lowByte) + x;

    if (valueAddress & 0xFF00 != highByte)
    {
        cycles++;
    }

    currentValue = read(valueAddress);
}

void CPU6502::absoluteY()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t highByte = read(pc);
    uint16_t valueAddress = ((highByte << 8) | lowByte) + y;

    if (valueAddress & 0xFF00 != highByte)
    {
        cycles++;
    }

    currentValue = read(valueAddress);
}

void CPU6502::zeroPage()
{
    currentAddress = pc++;
    currentValue = read(read(pc) & 0xFF);
}   

void CPU6502::zeroPageX()
{
    currentAddress = pc++;
    currentValue = read((read(pc) + x) & 0xFF);
}

void CPU6502::zeroPageY()
{
    currentAddress = pc++;
    currentValue = read((read(pc) + y) & 0xFF);
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
    uint8_t result = a & currentValue;

    setFlag(CPUFLAGS::Z, result == 0x00);
    setFlag(CPUFLAGS::N, result & 0x80);
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
}

void CPU6502::BCC()
{

}

void CPU6502::BCS()
{

}

void CPU6502::BEQ()
{

}

void CPU6502::BIT()
{

}

void CPU6502::BMI()
{

}

void CPU6502::BNE()
{

}

void CPU6502::BPL()
{

}

void CPU6502::BRK()
{

}

void CPU6502::BVC()
{

}

void CPU6502::BVS()
{

}

void CPU6502::CLC()
{

}

void CPU6502::CLD()
{

}

void CPU6502::CLI()
{

}

void CPU6502::CLV()
{

}

void CPU6502::CMP()
{

}

void CPU6502::CPX()
{

}

void CPU6502::CPY()
{

}

void CPU6502::DEC()
{

}

void CPU6502::DEX()
{

}

void CPU6502::DEY()
{

}

void CPU6502::EOR()
{

}

void CPU6502::INC()
{

}

void CPU6502::INX()
{

}

void CPU6502::INY()
{

}

void CPU6502::JMP()
{

}

void CPU6502::JSR()
{

}

void CPU6502::LDA()
{

}

void CPU6502::LDX()
{

}

void CPU6502::LDY()
{

}

void CPU6502::LSR()
{

}

void CPU6502::NOP()
{

}

void CPU6502::ORA()
{

}

void CPU6502::PHA()
{

}

void CPU6502::PHP()
{

}

void CPU6502::PLA()
{

}

void CPU6502::PLP()
{

}

void CPU6502::ROL()
{

}

void CPU6502::ROR()
{

}

void CPU6502::RTI()
{

}

void CPU6502::RTS()
{

}

void CPU6502::SBC()
{

}

void CPU6502::SEC()
{

}

void CPU6502::SED()
{

}

void CPU6502::SEI()
{

}

void CPU6502::STA()
{

}

void CPU6502::STX()
{

}

void CPU6502::STY()
{

}

void CPU6502::TAX()
{

}

void CPU6502::TAY()
{

}

void CPU6502::TSX()
{

}

void CPU6502::TXA()
{

}

void CPU6502::TXS()
{

}

void CPU6502::TYA()
{

}
