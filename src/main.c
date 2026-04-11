#include <curses.h>
#include <stdio.h>

int main(void)
{
    // game steps
    // ask for length and width
    // ask for safe first click
    // game begin;
    // print board
    // wait for actions
    // process actions
    // loop to print board if not the end
    // end game

    initscr();
    printw("hello world\n");
    move(10, 10);
    printw("la suite ici");
    getch();
    endwin();
    return 0;
}
