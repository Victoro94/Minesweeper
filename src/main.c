#include <curses.h>
#include <stddef.h>
#include <stdlib.h>

#include "board.h"

// game variables
WINDOW *window = NULL;
int BOARD_LENGTH = 0;
int BOARD_WIDTH = 0;
int BOMB_NUMBER = 0;
int POS_X = 0;
int POS_Y = 0;

int SAFE_FIRST_CLICK = TRUE;

struct cell **BOARD = NULL;

char *readline(char *str)
{
    printf("%s", str);
    fflush(stdout);

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        free(line);
        return NULL;
    }

    if (nread > 0 && line[nread - 1] == '\n')
        line[nread - 1] = '\0';
    return line;
}

int set_game_options(void)
{
    // sets game options
    // return 0 if no error and 1 if one happened

    // getting length
    char *length = readline("choose a length :");
    BOARD_LENGTH = atoi(length);
    free(length);
    if (BOARD_LENGTH == 0)
    {
        printf("length can't be 0 or couldn't parse given number\n");
        return 1;
    }

    // getting width
    char *width = readline("choose a width :");
    BOARD_WIDTH = atoi(width);
    free(width);
    if (BOARD_WIDTH == 0)
    {
        printf("width can't be 0 or couldn't parse given number\n");
        return 1;
    }

    // getting bomb number
    //  propose a bomb number depending of board ?
    char *bomb_number = readline("how much bombs ? :");
    BOMB_NUMBER = atoi(bomb_number);
    free(bomb_number);
    if (BOMB_NUMBER == 0)
    {
        printf("bomb number can't be 0 or couldn't parse given number\n");
        return 1;
    }

    // ask for safe first click
    return 0;
}

int process_input(void)
{
    int input = getch();
    printw("%d", input);
    switch (input)
    {
    case 'q':
        return 1;
    case KEY_UP:
        if (POS_Y > 0)
            POS_Y--;
        break;
    case KEY_DOWN:
        if (POS_Y < BOARD_WIDTH - 1)
            POS_Y++;
        break;
    case KEY_LEFT:
        if (POS_X > 0)
            POS_X--;
        break;
    case KEY_RIGHT:
        if (POS_X < BOARD_LENGTH - 1)
            POS_X++;
        break;
    default:
    }
    return 0;
}

int init_board(void)
{
    size_t cell_number = BOARD_LENGTH * BOARD_WIDTH;
    BOARD = calloc(cell_number, sizeof(struct cell *));
    if (BOARD == NULL)
        return 1;
    for (size_t i = 0; i < cell_number; i++)
    {
        BOARD[i] = calloc(1, sizeof(struct cell));
        if (BOARD[i] == NULL)
            return 1;
    }
    return 0;
}

void free_board(void)
{
    size_t cell_number = BOARD_LENGTH * BOARD_WIDTH;
    if (BOARD)
    {
        for (size_t i = 0; i < cell_number; i++)
            free(BOARD[i]);
        free(BOARD);
    }
}

int main(void)
{
    // game steps :
    // ask for length and width done
    // ask for safe first click
    // game begin;
    // print board
    // wait for actions
    // process actions
    // loop to print board if not the end
    // end game

    // game begins

    int options = set_game_options();
    if (options == 1)
        return 1;
    int board = init_board();
    if (board == 1)
        return 1;
    window = initscr();
    noecho();
    keypad(window, TRUE);
    int stop = 0;
    while (!stop)
    {
        // print board
        clear();
        print_board(BOARD, BOARD_LENGTH, BOARD_WIDTH);
        refresh();
        printw("POS_X : %d, POS_Y : %d\n", POS_X, POS_Y);
        move(1 + 2 * POS_Y, 2 + 4 * POS_X);
        stop = process_input();
    }
    free_board();
    endwin();
    return 0;
}
