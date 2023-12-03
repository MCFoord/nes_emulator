#include <ncurses.h>
#include "interface.h"

WINDOW* initialise()
{
    WINDOW* win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    initscr();
    noecho();
    curs_set(0);
    return win;
}

void getInput()
{
    int ch = getch();

    switch (ch)
    {
    case 'q':
        break;
    
    default:
        break;
    }
}

void displayOptions()
{
    addstr("-----------6502 debug-----------\n\n");
    addstr("   options:\n");
    addstr("       - e to execute next instruction\n");
    addstr("       - q to quit\n");
    addstr("       - r to reset\n\n");
    addstr("--------------------------------\n");
}

void displayRegisters()
{

}

void displayMemory()
{

}
