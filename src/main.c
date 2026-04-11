#include <curses.h>
#include <stddef.h>
#include <stdlib.h>
int BOARD_LENGTH = 0;
int BOARD_WIDTH = 0;
int BOMB_NUMBER = 0;
int SAFE_FIRST_CLICK = FALSE;
int *BOARD = NULL;

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

int set_game_options()
{
    // sets game options
    // return 0 if no error and 1 if one happened

    // getting length
    char *length = readline("choose a length :");
    BOARD_LENGTH = atoi(length);
    if (BOARD_LENGTH == 0)
    {
        printf("length can't be 0 or couldn't parse given number\n");
        return 1;
    }
    free(length);

    // getting width
    char *width = readline("choose a width :");
    BOARD_WIDTH = atoi(width);
    if (BOARD_WIDTH == 0)
    {
        printf("width can't be 0 or couldn't parse given number\n");
        return 1;
    }
    free(width);

    // getting bomb number
    //  propose a bomb number depending of board ?
    char *bomb_number = readline("how much bombs ? :");
    BOMB_NUMBER = atoi(bomb_number);
    if (BOMB_NUMBER == 0)
    {
        printf("bomb number  can't be 0 or couldn't parse given number\n");
        return 1;
    }
    free(bomb_number);

    // ask for safe first click
    return 0;
}

int process_input()
{
    int input = getch();
    clear();
    printw("%d\n", input);
    return 0;
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

    // int options = set_game_options();

    // init board

    initscr();
    int stop = 0;
    while (!stop)
    {
        // print board
        // get input
        // process input
        stop = process_input();
        refresh(); // Rafraîchit la fenêtre par défaut (stdscr) afin d'afficher
                   // le message
    }

    endwin();
    return 0;
}
