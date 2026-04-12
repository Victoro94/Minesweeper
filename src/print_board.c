#include <ncurses.h>

#include "board.h"

void print_row(int length)
{
    for (int i = 0; i < length; i++)
    {
        printw("+---");
    }
    printw("+\n");
}

void print_mines(int length)
{
    for (int i = 0; i < length; i++)
    {
        printw("| ");
        printw("x");
        printw(" ");
    }
    printw("|\n");
}
int print_board(struct cell **board, int length, int width)
{
    // TODO add colors
    // add information about game
    if (board)
        board[0]->state = HIDDEN;
    for (int i = 0; i < width; i++)
    {
        print_row(length);
        print_mines(length);
    }
    print_row(length);
    return 0;
}
