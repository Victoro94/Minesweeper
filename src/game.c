#include "board.h"
#include "game.h"

#include <stdlib.h>
#include <time.h>

void update_cell(struct cell **board, int x , int y, int length, int width)
{
    if (x < 0 || y < 0 || x >=length || y >= width)
        return ;
    int current = board[y * length+x] -> bomb_around;
    board[y * length + x] -> bomb_around = current+1;
}

void update_neighbors(struct cell **board, int length, int width)
{
    for (int x = 0 ; x < length; x++)
    {
        for (int y = 0; y < width; y++)
        {
            if (board[y*length+x]-> is_bomb ==1)
            {
                update_cell(board,x-1,y-1,length,width);
                update_cell(board,x-1,y,length,width);
                update_cell(board,x-1,y+1,length,width);
                update_cell(board,x,y-1,length,width);
                update_cell(board,x,y+1,length,width);
                update_cell(board,x+1,y-1,length,width);
                update_cell(board,x+1,y,length,width);
                update_cell(board,x+1,y+1,length,width);
            }
        }
    }

}

int put_mines(struct cell **board, int bomb_number, int length, int width)
{
    // build cell list
    // shuffle list
    // pick n first correct cell
    // put mines in each cell and update his neighbors   
    int bomb_placed = 0;
    int board_length = length * width;
    while (bomb_placed < bomb_number)
    {
        int i = 0;
        while (bomb_placed < bomb_number && i < board_length)
        {
            //if () condition for anti-clustering
            board[i] -> is_bomb = 1;
            bomb_placed++;
            i++;
        }
        
    }
    update_neighbors(board,length,width);
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
