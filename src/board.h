#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

enum STATE
{
    HIDDEN,
    FLAGGED,
    SHOWN
};

struct cell
{
    enum STATE state;
    bool is_bomb;
    int bomb_around;
};

struct cell **board_init(int length, int width);
void board_free(struct cell **board, int length, int width);

int board_print(struct cell **board, int length, int width);
void show_bombs(struct cell **board, int length, int width,enum STATE state);
#endif /* ! BOARD_H */
