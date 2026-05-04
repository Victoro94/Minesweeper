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
char* game_status = NULL;
int SEED = 1;
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
        return -2;
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
        if (REVEALED == 0)
        {
            if (SAFE_FIRST_CLICK == TRUE)
                BOARD[POS_Y * BOARD_LENGTH + POS_X] -> state = SHOWN;
            put_mines(BOARD, BOMB_NUMBER, BOARD_LENGTH, BOARD_WIDTH,SEED);
            BOARD[POS_Y * BOARD_LENGTH + POS_X] -> state = HIDDEN;
        }
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

int check_win(int status)
{
    if (status > 0)
    {
        if (REVEALED == -1)
        {
            game_status = "YOU LOST, MAYBE NEXT TIME.";
            show_bombs(BOARD,BOARD_LENGTH,BOARD_WIDTH);
            return -1;
        }
        else if (REVEALED == BOARD_LENGTH * BOARD_WIDTH - BOMB_NUMBER)
        {
            game_status = "YOU WIN !!";
            flag_bombs(BOARD,BOARD_LENGTH,BOARD_WIDTH);
            return -1;
        }
    }
    return status;
}

int main(void)
{
    int options = set_game_options();
    if (options == 1)
        return 1;
    BOARD = board_init(BOARD_LENGTH,BOARD_WIDTH);
    if (!BOARD)
        return 1;
    window = initscr();
    start_color();
    if (COLORS > 8)
        define_colors();
    noecho();
    keypad(window, TRUE);
    // game begins
    //first print
    int stop = 1;
    board_print(BOARD, BOARD_LENGTH, BOARD_WIDTH);
    printw("Commands :\n");
    printw("q: quit  ENTER: show  ARROWS: move curser   BACKSPACE: flag\n");
    printw("revealed : %d/%d\nbomb number: %d\n",REVEALED,BOARD_LENGTH*BOARD_WIDTH,BOMB_NUMBER);
    move(1, 2);// first case

    //safe click 
    while (stop >= 0)
    {
        // print board
        stop = process_input();
        stop = check_win(stop);
        // print board
        if(stop != 0)
        {
            clear();
            board_print(BOARD, BOARD_LENGTH, BOARD_WIDTH);
            refresh();
            // print here for commands
            printw("Commands :\n");
            printw("q: quit  ENTER: show  ARROWS: move curser   BACKSPACE: flag\n");
            printw("revealed : %d/%d\nbomb number: %d\n",REVEALED,BOARD_LENGTH*BOARD_WIDTH,BOMB_NUMBER);
            //printw("board : %d",BOARD[0] -> state);
            move(1 + 2 * POS_Y, 2 + 4 * POS_X);
        }
    }
    if (stop == -1)
    {
        clear();
        board_print(BOARD, BOARD_LENGTH, BOARD_WIDTH);
        refresh();
        // print here for commands
        printw("%s\n",game_status);
        printw("revealed: %d/%d\nbomb number: %d\n",REVEALED,BOARD_LENGTH*BOARD_WIDTH,BOMB_NUMBER);
        printw("Click anywhere to quit");
        getch();
    }   
    board_free(BOARD, BOARD_LENGTH, BOARD_WIDTH);
    endwin();
    return 0;
}
