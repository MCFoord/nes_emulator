#include "cpu6502.h"
#include "bus.h"

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
    uint16_t valueAddress = (read(pc) << 8) | lowByte;

    currentValue = read(valueAddress + x);
}

void CPU6502::absoluteY()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t valueAddress = (read(pc) << 8) | lowByte;

    currentValue = read(valueAddress + y);
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
