#ifndef GAME_H
#define GAME_H
#include "board.h"
#include <stddef.h>

struct game 
{
    struct cell **board;
    size_t length;
    size_t width;
    size_t bomb_number;
    bool safe_first_click;
    //safe first click pos
    size_t x;
    size_t y;
    int status;
    unsigned int seed;
    // 0 ok 
    // -1 length problem
    // -2 width pb
    // -3 bomb number problem
    // -4 seed problem
    // 1 asked help (print help)
};
struct game *parse_options(int argc, char *argv[]);

int put_mines(struct game *game);

int reveal(struct game *game, int x, int y);

int process_input(struct game *game, size_t *x, size_t *y, int *revealed, int *flags);

#endif /* ! GAME_H */
