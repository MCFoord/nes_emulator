#include <iostream>
#include <ncurses.h>
#include <mos6502.h>
#include <bus.h>
// #include "interface.h"

#define WINDOW_HEIGHT 50
#define WINDOW_WIDTH 50

int main(int argc, char **argv)
{
    switch (argc)
    {
    case 1:
        std::cout << "[ERROR] binary file must be provided";
        return EXIT_FAILURE;
        break;

    case 2:
        break;
    
    default:
        std::cout << "[ERROR] too many arguments provided";
        return EXIT_FAILURE;
        break;
    }

    mos6502* cpu = new mos6502();
    Bus* bus = new Bus();

    cpu->connectBus(bus);
    bus->loadProgram(argv[1]);
    cpu->Reset();

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
        addstr("       - q to quit\n");
        addstr("       - r to reset\n\n");
        addstr("--------------------------------------------------------\n\n\n");
        refresh();

        // cpu->fetch();
        addstr("instruction Count: ");
        addstr(std::to_string(instructionCount).c_str());
        addstr("\n");
        addstr("Registers:\n\n");
        addstr(cpu->registerToString().c_str());
        addstr("\n\n");
        refresh();

        addstr("Status:\n\n");
        addstr(cpu->statusToString().c_str());
        addstr("\n\n");
        refresh();

        addstr("Zero Page:\n\n");
        addstr(bus->memToString(0x0000, 0x00FF).c_str());
        addstr("\n\n");
        refresh();

        addstr("Stack:\n\n");
        addstr(bus->memToString(0x0100, 0x01FF).c_str());
        addstr("\n\n");

        addstr("Program Data:\n\n");
        addstr(bus->memToString(0x0400, 0x04FF).c_str());
        addstr("\n\n");

        refresh();

        int ch = getch();

        switch (ch)
        {
        case 'e':
            cpu->execute();
            instructionCount++;
            break;
            
        case 'q':
            quit = true;
            break;
        
        case 'r':
            bus->loadProgram(argv[1]);
            cpu->Reset();
            instructionCount = 0;
            break;
        
        default:
            break;
        }
    }

    endwin();
    return 0;
}