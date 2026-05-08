#include <curses.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game.h"

// game variables
WINDOW *window = NULL;
size_t POS_X = 0;
size_t POS_Y = 0;
int REVEALED = 0;
char* game_status = NULL;

void define_colors(void)
{
    init_pair(1,COLOR_BLACK,COLOR_BLACK); // for empty cells
    init_pair(2,COLOR_WHITE,COLOR_WHITE); // for hidden cells
    init_pair(3,COLOR_BLACK,COLOR_WHITE); // for flagged cells
    init_pair(4,COLOR_YELLOW,COLOR_RED); // bomb found
    init_pair(5,COLOR_BLUE,COLOR_BLACK); // other (for now)
}

int process_input(struct game *game)
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
        if (POS_Y < game->width - 1)
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
        if (POS_X < game->length - 1)
        {
            POS_X++;
            print = 1;
        }
        break;
    case KEY_BACKSPACE :
        int pos = game->length * POS_Y + POS_X;
        if (game->board[pos] -> state != SHOWN)
        {
            if (game->board[pos] -> state == FLAGGED)
                game->board[pos] -> state = HIDDEN;
            else
                game->board[pos] -> state = FLAGGED;
            print = 1; 
        }
        break;  
    case 10 :
        if (REVEALED == 0)
        {
            if (game->safe_first_click == true)
            {
                game->board[POS_Y * game->length + POS_X] -> state = SHOWN;
                game -> x = POS_X;
                game -> y = POS_Y;
            }
            put_mines(game);
            game->board[POS_Y * game->length + POS_X] -> state = HIDDEN;
        }
        int revealed = reveal(game,POS_X,POS_Y);
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

int check_win(struct game *game,int status)
{
    if (status > 0)
    {
        if (REVEALED == -1)
        {
            game_status = "YOU LOST, MAYBE NEXT TIME.";
            show_bombs(game->board,game->length,game->width);
            return -1;
        }
        else if ((size_t)REVEALED == game->length * game->width - game->bomb_number)
        {
            game_status = "YOU WIN !!";
            flag_bombs(game->board,game->length,game->width);
            return -1;
        }
    }
    return status;
}

void print_help()
{
    puts("name : Minesweeper - simple minesweeper game on the terminal\n\n");
    puts("usage: ./Minesweeper [length] [width] [bomb number] [seed] "
    "[safe_first_click]\n");
    puts("\tlength:       integer higher than 0");
    puts("\twidth:        integer higher than 0");
    puts("\tbomb_number:  integer higher than 0");
    puts("\tseed:         integer higher or equal to 0");
    puts("\tsafe_first_click: f,F,false,False or FALSE\n\n");
    puts("default values:\n");
    puts("\tlength and width:  10");
    puts("\tbomb_number:       20");
    puts("\tseed:              0(random using time)");
    puts("\tsafe_first_click:  true");
}

void print_status(struct game *game)
{
    switch (game -> status)
    {
        case 1 :
            print_help();
            return;
        case -1:
            fprintf(stderr,"grid length should be a number higher than 0\n");
            break;
        case -2:
            fprintf(stderr,"grid width should be a number higher than 0\n");
            break;
        case -3:
            fprintf(stderr,"bomb number should be a number higher than 0\n");
            break;
        case -4:
            fprintf(stderr,"seed should be a number greater or equal to 0\n");
    }
    fprintf(stderr,"use -h for more help\n");
}

int main(int argc, char *argv[])
{
    struct game *game = parse_options(argc,argv);
    if (game == NULL)
    {
        fprintf(stderr,"allocation failed, try closing some apps.\n");
        return 1;
    }
    if (game -> status != 0)
    {
        print_status(game);
        return 0;
    }
    game -> board = board_init(game->length, game->width);
    if (!game -> board)
    {
        fprintf(stderr,"allocation failed, try closing some apps.\n");
        return 1;
    }
    if (game->seed == 0)
    {
        // set a random seed using time 
        game -> seed = time(NULL);
    }
    printw("%d\n",game->seed);
    window = initscr();
    start_color();
    if (COLORS > 8)
        define_colors();
    noecho();
    keypad(window, TRUE);
    // game begins
    //first print
    int stop = 1;
    board_print(game->board, game->length, game->width);
    printw("Commands :\n");
    printw("q: quit  ENTER: show  ARROWS: move curser   BACKSPACE: flag\n");
    printw("revealed: %d/%ld\nbomb number: %ld\n",REVEALED,game->length*game->width,game->bomb_number);
    printw("seed: %d\n",game->seed);
    move(1, 2);// first case

    //safe click 
    while (stop >= 0)
    {
        // print board
        stop = process_input(game);
        stop = check_win(game,stop);
        // print board
        if(stop != 0)
        {
            //clear();
            move(0,0);
            board_print(game->board, game->length, game->width);
            refresh();
            // print here for commands
            printw("Commands :\n");
            printw("q: quit  ENTER: show  ARROWS: move curser   BACKSPACE: flag\n");
            printw("revealed : %d/%ld\nbomb number: %ld\n",REVEALED,game->length*game->width,game->bomb_number);
            printw("seed: %d\n",game->seed);
            //printw("board : %d",BOARD[0] -> state);
            move(1 + 2 * POS_Y, 2 + 4 * POS_X);
        }
    }
    if (stop == -1)
    {
        clear();
        board_print(game->board,game->length,game->width);
        refresh();
        // print here for commands
        printw("%s\n",game_status);
        printw("revealed: %d/%ld\nbomb number: %ld\n",REVEALED,game->length*game->width,game->bomb_number);
        printw("seed: %d\n",game->seed);
        printw("Click anywhere to quit");
        getch();
    }   
    board_free(game->board, game->length, game->width);
    endwin();
    return 0;
}
