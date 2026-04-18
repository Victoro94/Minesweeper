#include <curses.h>
#include <stddef.h>
#include <stdlib.h>

#include "board.h"
#include "game.h"

// game variables
WINDOW *window = NULL;
int BOARD_LENGTH = 0;
int BOARD_WIDTH = 0;
int BOMB_NUMBER = 0;
int POS_X = 0;
int POS_Y = 0;
int REVEALED = 0;

int SAFE_FIRST_CLICK = TRUE;

struct cell **BOARD = NULL;


void define_colors(void)
{
    init_pair(1,COLOR_BLACK,COLOR_BLACK); // for empty cells
    init_pair(2,COLOR_WHITE,COLOR_WHITE); // for hidden cells
    init_pair(3,COLOR_BLACK,COLOR_WHITE); // for flagged cells
    init_pair(4,COLOR_YELLOW,COLOR_RED); // bomb found
    init_pair(5,COLOR_BLUE,COLOR_BLACK); // other (for now)

}

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

        printw("number of color: %d\n",COLORS);
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
    //printw("%d", input);
    int print = 0;
    switch (input)
    {
    case 'q':
        return -1;
    case KEY_UP:
        if (POS_Y > 0)
        {
            POS_Y--;
            print = 1;
        }
        break;
    case KEY_DOWN:
        if (POS_Y < BOARD_WIDTH - 1)
        {
            POS_Y++;
            print = 1;
        }
        break;
    case KEY_LEFT:
        if (POS_X > 0)
        {
            print = 1;
            POS_X--;
        }
        break;
    case KEY_RIGHT:
        if (POS_X < BOARD_LENGTH - 1)
        {
            POS_X++;
            print = 1;
        }
        break;
    case KEY_BACKSPACE :
        int pos = BOARD_LENGTH * POS_Y + POS_X;
        if (BOARD[pos] -> state != SHOWN)
        {
            if (BOARD[pos] -> state == FLAGGED)
                BOARD[pos] -> state = HIDDEN;
            else
                BOARD[pos] -> state = FLAGGED;
            print = 1; 
        }
        break;  
    case 10 :
        int revealed = reveal(BOARD,POS_X,POS_Y,BOARD_LENGTH,BOARD_WIDTH);
        if (revealed == -1)
            REVEALED = -1;//lose
        else
            REVEALED+=revealed;
        if (revealed != 0)
            print = 1;
        break;
    default:
    }
    if (print == 1)
        return 1;
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
        BOARD[i] -> state = HIDDEN;
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

int check_win(int status)
{
    if (status > 0)
    {
        if (REVEALED == -1)
        {


        }
        else if (REVEALED == BOARD_LENGTH * BOARD_WIDTH)
        {


        }
    }
    return status;
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
    //tests
    BOARD[0] -> state = FLAGGED;
    BOARD[1] -> state = SHOWN;
    BOARD[2] -> state = SHOWN;
    BOARD[2] -> is_bomb = 1;
    BOARD[3] -> state = HIDDEN;
    BOARD[3] -> bomb_around = 3;
    BOARD[4] -> state = FLAGGED;
    // end of tests
    window = initscr();
    start_color();
    if (COLORS > 8)
        define_colors();
    noecho();
    keypad(window, TRUE);
    int stop = 1;  
    while (stop >= 0)
    {
        // print board
        if(stop != 0)
        {
            clear();
            print_board(BOARD, BOARD_LENGTH, BOARD_WIDTH);
            refresh();
            // print here for commands
            printw("Commands :\n");
            printw("q: quit  ENTER: show  ARROWS: move curser   BACKSPACE: flag\n");

            //printw("board : %d",BOARD[0] -> state);
            move(1 + 2 * POS_Y, 2 + 4 * POS_X);
        }
        stop = process_input();
        //check win
        stop = check_win(stop);
    }
    /* print colors 
    for(int i  = 0 ;i < 256; i++)
    {
        init_pair(i+3,i,COLOR_BLACK);
        attron(COLOR_PAIR(i+3));
        printw("%d ",i);
    }
    getch();
    */
    getch()
    free_board();
    endwin();
    return 0;
}
