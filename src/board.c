#include <ncurses.h>
#include <stdlib.h>
#include "board.h"

struct cell **board_init(int length, int width)
{
    size_t cell_number = length * width;
    struct cell ** board= calloc(cell_number, sizeof(struct cell *));
    if (board == NULL)
        return NULL;
    for (size_t i = 0; i < cell_number; i++)
    {
        board[i] = calloc(1, sizeof(struct cell));
        if (board[i] == NULL)
            return NULL;
        board[i] -> state = HIDDEN;
    }
    return board;
}

void board_free(struct cell **board, int length ,int width )
{
    size_t cell_number = length * width;
    if (board)
    {
        for (size_t i = 0; i < cell_number; i++)
            free(board[i]);
        free(board);
    }
}

static int set_color(struct cell *cell)
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

static void print_cell(struct cell *cell)
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

static void print_row(int length)
{
    attron(COLOR_PAIR(0));
    for (int i = 0; i < length; i++)
    {
        printw("+---");
    }
    printw("+\n");
}

static void print_mines(struct cell **board, int index, int length)
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

int board_print(struct cell **board, int length, int width)
{
    // TODO add colors
    for (int i = 0; i < width; i++)
    {
        print_row(length);
        print_mines(board, i, length);
    }
    print_row(length);
    return 0;
}

void show_bombs(struct cell **board, int length, int width, enum STATE state)
{
    for (int x = 0; x < length; x++)
    {
        for (int y = 0; y < width; y++)
        {
            if (board[y * length + x] -> is_bomb == 1)
                board[y * length + x] -> state = state;
        }
    }

}
