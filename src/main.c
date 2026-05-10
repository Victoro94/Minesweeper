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
int FLAGS = 0;
char* game_status = NULL;

void define_colors(void)
{
    init_pair(1,COLOR_BLACK,COLOR_BLACK); // for empty cells
    init_pair(2,COLOR_WHITE,COLOR_WHITE); // for hidden cells
    init_pair(3,COLOR_BLACK,COLOR_WHITE); // for flagged cells
    init_pair(4,COLOR_YELLOW,COLOR_RED); // bomb found
    init_pair(5,COLOR_BLUE,COLOR_BLACK); // other (for now)
}

int check_win(struct game *game,int status)
{
    if (status > 0)
    {
        if (REVEALED == -1)
        {
            game_status = "YOU LOST, MAYBE NEXT TIME.";
            show_bombs(game->board,game->length,game->width,SHOWN);
            return -1;
        }
        else if ((size_t)REVEALED == game->length * game->width - game->bomb_number)
        {
            game_status = "YOU WIN !!";
            show_bombs(game->board,game->length,game->width,FLAGGED);
            FLAGS = game -> bomb_number;
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

void print_all(struct game* game, int stop)
{
    move(0,0);
    int width =  game-> width;
    int length = game -> length;
    board_print(game->board, length, width);
    refresh();
    if (stop >= 0 )
    {
        printw("Commands :\n");
        printw("q: quit  ENTER: show  ARROWS: move curser   BACKSPACE: flag\n");
    }
    else
        printw("%s\n",game_status);
    printw("revealed: %d/%d ",REVEALED,length * width);
    printw("flagged: %d\n",FLAGS);
    printw("bomb number: %ld\n",game->bomb_number);
    printw("seed: %d\n",game->seed);
    if (stop == -1)
    { 
        printw("Click anywhere to quit");
    }
    move(1 + 2 * POS_Y, 2 + 4 * POS_X);
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
    window = initscr();
    start_color();
    if (COLORS > 8)
        define_colors();
    noecho();
    keypad(window, TRUE);
    // game begins
    //first print
    int stop = 1;
    print_all(game,stop); 
    while (stop >= 0)
    {
        stop = process_input(game, &POS_X, &POS_Y, &REVEALED,&FLAGS);
        stop = check_win(game,stop);
        if(stop != 0)
        {
            //need to print the board
            print_all(game,stop);
        }
    }
    if (stop == -1)
    {
        print_all(game,stop);
        getch();
    }   
    board_free(game->board, game->length, game->width);
    endwin();
    return 0;
}
