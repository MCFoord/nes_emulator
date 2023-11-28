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

}

void displayRegisters()
{

}

void displayMemory()
{

}
