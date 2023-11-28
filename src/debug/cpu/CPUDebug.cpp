#include <iostream>
#include <ncurses.h>
#include "interface.h"

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
        std::cout << "[ERROR] too many arguments provided";\
        return EXIT_FAILURE;
        break;
    }

    WINDOW* win = initialise();

    
}