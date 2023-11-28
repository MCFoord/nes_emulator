#pragma once
#include <ncurses.h>

#define WINDOW_HEIGHT 50
#define WINDOW_WIDTH 50

WINDOW* initialise();
void getInput(WINDOW* win);
void displayOptions(WINDOW* win);
void displayRegisters(WINDOW* win);
void displayMemory(WINDOW* win);