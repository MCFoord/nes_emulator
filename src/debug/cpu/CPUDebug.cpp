#include <iostream>
#include <ncurses.h>
#include <cpu6502.h>
#include <bus.h>
#include <fstream>
#include "mos6502.h"

#define WINDOW_HEIGHT 50
#define WINDOW_WIDTH 50

int main(int argc, char **argv)
{
    std::ofstream debugOutput;

    switch (argc)
    {
    case 1:
        std::cout << "[ERROR] binary file must be provided";
        return EXIT_FAILURE;
        break;

    case 2:
        break;

    case 3:
        debugOutput.open(argv[2]);
        if (!debugOutput.is_open())
        {
            std::cout << "[ERROR] output file could not be opened";
            return EXIT_FAILURE;
        }
        break;
    
    default:
        std::cout << "[ERROR] too many arguments provided";
        return EXIT_FAILURE;
        break;
    }

    CPU6502* cpu1 = new CPU6502();
    mos6502* cpu2 = new mos6502();
    Bus* bus1 = new Bus();
    Bus* bus2 = new Bus();

    cpu1->connectBus(bus1);
    bus1->loadProgram(argv[1]);
    cpu1->reset();

    cpu2->connectBus(bus2);
    bus2->loadProgram(argv[1]);
    cpu2->Reset();


    WINDOW* win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    initscr();
    noecho();
    curs_set(0);

    bool quit = false;

    int instructionCount = 0;
    while (!quit)
    {
        erase();
        addstr("---------------------- 6502 debug ----------------------\n\n");
        addstr("   options:\n");
        addstr("       - e to execute next instruction\n");
        addstr("       - a to run until illegal operation is encountered\n");
        addstr("       - r to reset\n");
        addstr("       - q to quit\n\n");
        addstr("--------------------------------------------------------\n\n\n");
        refresh();

        cpu1->fetch();
        addstr("instruction Count: ");
        addstr(std::to_string(instructionCount).c_str());
        addstr("\n");
        addstr("Registers:\n\n");
        addstr(cpu1->registerToString().c_str());
        addstr("\n");
        addstr(cpu1->instructionInfoToString().c_str());
        addstr("\n\n");
        refresh();

        addstr("Status:\n\n");
        addstr(cpu1->statusToString().c_str());
        addstr("\n\n");
        refresh();

        addstr("Zero Page:\n\n");
        addstr(bus1->memToString(0x0000, 0x00FF).c_str());
        addstr("\n\n");
        refresh();

        addstr("Stack:\n\n");
        addstr(bus1->memToString(0x0100, 0x01FF).c_str());
        addstr("\n\n");

        addstr("Program Data:\n\n");
        addstr(bus1->memToString(0x0400, 0x04FF).c_str());
        addstr("\n\n");

        refresh();

        int ch = getch();
        int count = 0;
        switch (ch)
        {
        case 'e':
            cpu1->execute();
            instructionCount++;
            break;
            
        case 'q':
            quit = true;
            break;

        case 'a':
            
            while
            (
                cpu1->a == cpu2->GetA() && cpu1->x == cpu2->GetX() &&
                cpu1->y == cpu2->GetY() && cpu1->pc == cpu2->GetPC() &&
                cpu1->sp == cpu2->GetS() && cpu1->status == cpu2->GetP()
            )
            {
                erase();
                addstr(std::to_string(count).c_str());
                addstr(" instructions run successfully");
                refresh();
                cpu1->execute();
                cpu2->execute();
                ++count;
            }

            cpu1->printOperation(cpu1->pc, debugOutput);
            cpu2->printOperation(cpu2->GetPC(), debugOutput);
            break;

        case 'r':
            bus1->loadProgram(argv[1]);
            cpu1->reset();
            instructionCount = 0;
            break;
        
        default:
            break;
        }
    }

    endwin();
    return 0;
}