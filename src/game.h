#ifndef GAME_H
#define GAME_H

int put_mines(struct cell **board, int bomb_number, int length ,int width);

int reveal(struct cell **board, int x, int y, int length, int width);

#endif /* ! GAME_H */
