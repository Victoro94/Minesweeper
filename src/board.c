#include <ncurses.h>

#include "board.h"


int set_color(struct cell *cell)
{
    if (COLORS>8)
    {
        if (cell->state == HIDDEN)
            return 2;
        else if (cell -> state == FLAGGED)
            return 3;
        else 
        {
            if (cell -> is_bomb == 1)
                return 4;
            else if (cell -> bomb_around == 0)
                return 1;
            else
                return 5;
        }
    }
    return 0;
}

void print_cell(struct cell *cell)
{
    //print the cell
    if (cell -> state == HIDDEN)
        printw(" ? ");
    else if (cell -> state == FLAGGED)
        printw(" X ");
    else 
    {
        if (cell -> is_bomb == 1)
            printw(" B ");
        else if (cell -> bomb_around == 0)
            printw("   ");
        else
            printw(" %d ",cell -> bomb_around);
    }
}

void print_row(int length)
{
    attron(COLOR_PAIR(0));
    for (int i = 0; i < length; i++)
    {
        printw("+---");
    }
    printw("+\n");
}

void print_mines(struct cell **board, int index, int length)
{
    for (int j = 0; j < length; j++)
    {
        attron(COLOR_PAIR(0));
        printw("|");
        int color = set_color(board[index * length + j]);
        attron(COLOR_PAIR(color));
        print_cell(board[index * length + j]);
        attroff(COLOR_PAIR(color));
    }
    attron(COLOR_PAIR(0));
    printw("|\n");
}
int print_board(struct cell **board, int length, int width)
{
    // TODO add colors
    // add information about game
    for (int i = 0; i < width; i++)
    {
        print_row(length);
        print_mines(board, i, length);
    }
    print_row(length);
    return 0;
}
