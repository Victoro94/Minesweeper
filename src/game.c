#include "board.h"
#include "game.h"

#include <stdlib.h>
#include <time.h>

int put_mines(struct cell ** board, int bomb_number, int length, int width)
{
    // build cell list
    // shuffle list
    // pick n first correct cell
    // put mines in each cell and update his neighbors   
    (void) board;
    (void) bomb_number;
    (void) length;
    (void) width;
    return 0;
}


int reveal(struct cell **board, int x, int y, int length, int width)
{
    if (x >=0 && x <length && y >= 0 && y < width)
    {
        int index = y *length +x;
        if (board[index] -> state == HIDDEN)
        {
            int revealed = 1;
            board[index] -> state = SHOWN;
            if (board[index] -> is_bomb)
                return -1;//bomb found
            // propagation
            if (board[index] -> bomb_around == 0 && board[index] -> is_bomb == 0)
            {
                revealed+=reveal(board,x-1,y-1,length,width);
                revealed+=reveal(board,x-1,y,length,width);
                revealed+=reveal(board,x-1,y+1,length,width);
                revealed+=reveal(board,x,y-1,length,width);
                revealed+=reveal(board,x,y+1,length,width);
                revealed+=reveal(board,x+1,y-1,length,width);
                revealed+=reveal(board,x+1,y,length,width);
                revealed+=reveal(board,x+1,y+1,length,width);
            }
            return revealed;
        }
    }
    return 0;
}
