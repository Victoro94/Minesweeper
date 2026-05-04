#ifndef GAME_H
#define GAME_H
#include "board.h"
int put_mines(struct cell **board, int bomb_number, int length ,int width, int seed);

int reveal(struct cell **board, int x, int y, int length, int width);

#endif /* ! GAME_H */
