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
    u_int8_t highByte;

    if (currentAddress & 0xFF == 0xFF)
    {
        read(currentAddress & 0xFF00);
    }
    else
    {
        read(pc);
    }

    //check whether the above bug still increases the program counter
    currentAddress = pc++;
    
    currentAddress = ( highByte << 8) | lowByte;

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
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t highByte = read(pc);

    currentAddress = ((highByte << 8) | lowByte) + y;
    
    if (currentAddress & 0xFF00 != highByte)
    {
        cycles++;
    }

    currentValue = read(currentAddress);
}

void CPU6502::absolute()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t currentAddress = (read(pc) << 8) | lowByte;

    currentValue = read(currentAddress);
}

void CPU6502::absoluteX()
{
    currentAddress = pc++;
    uint16_t lowByte = read(pc);
    currentAddress = pc++;
    uint16_t highByte = read(pc);
    currentAddress = ((highByte << 8) | lowByte) + x;

    if (currentAddress & 0xFF00 != highByte)
    {
        cycles++;
    }

    currentValue = read(currentAddress);
}

void CPU6502::absoluteY()
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

    currentValue = read(currentAddress);
}

/* check this later for any bugs to do with not actually
    assigning the value address to current address
*/
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
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
    }
}

void CPU6502::BCS()
{
    if (getFlag(CPUFLAGS::C))
    {
        cycles++;
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
    }
}

void CPU6502::BEQ()
{
    if (getFlag(CPUFLAGS::Z))
    {
        cycles++;
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
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
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
    }
}

void CPU6502::BNE()
{
    if (!getFlag(CPUFLAGS::Z))
    {
        cycles++;
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
    }
}

void CPU6502::BPL()
{
    if (!getFlag(CPUFLAGS::N))
    {
        cycles++;
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
    }
}

void CPU6502::BRK()
{
    push((pc >> 8) & 0xFF);
    push(pc & 0xFF);

    pc = (read(0xFFFF) << 8) | read(0xFFFE);
    setFlag(CPU6502::B, true);
    setFlag(CPU6502::I, true);
}

void CPU6502::BVC()
{
    if (!getFlag(CPUFLAGS::V))
    {
        cycles++;
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
    }
}

void CPU6502::BVS()
{
    if (getFlag(CPUFLAGS::V))
    {
        cycles++;
        uint16_t rel = pc + currentAddress;

        if (rel & 0xFF00 != pc & 0xFF00)
        {
            cycles++;
        }

        pc = rel;
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
    setFlag(CPU6502::Z, (a = currentValue));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::CPX()
{
    uint8_t result = x - currentValue;

    setFlag(CPU6502::C, (x >= currentValue));
    setFlag(CPU6502::Z, (x = currentValue));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::CPY()
{
    uint8_t result = y - currentValue;

    setFlag(CPU6502::C, (y >= currentValue));
    setFlag(CPU6502::Z, (y = currentValue));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::DEC()
{
    u_int8_t result = currentValue - 1;

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
    u_int8_t result = currentValue + 1;

    write(currentAddress, result);
    setFlag(CPU6502::Z, (result == 0x00));
    setFlag(CPU6502::N, (result & 0x80));
}

void CPU6502::INX()
{
    x--;

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
    uint8_t highByte = pop();
    uint8_t lowByte = pop();

    pc = (highByte << 8) | lowByte;
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
