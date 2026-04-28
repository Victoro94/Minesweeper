#ifndef BOARD_H
#define BOARD_H

enum STATE
{
    HIDDEN,
    FLAGGED,
    SHOWN
};

struct cell
{
    enum STATE state;
    int is_bomb;
    int bomb_around;
};

struct cell **board_init(int length, int width);
void board_free(struct cell **board, int length, int width);

int board_print(struct cell **board, int length, int width);
#endif /* ! BOARD_H */
