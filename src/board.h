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

int print_board(struct cell **board, int length, int width);
#endif /* ! BOARD_H */
