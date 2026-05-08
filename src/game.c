#include "board.h"
#include "game.h"

#include <string.h>
#include <stdlib.h>

struct game *parse_options(int argc, char *argv[])
{
    struct game *res = calloc(1,sizeof(struct game));
    if (res == NULL)
        return NULL;// memory problem
    res -> width = 10;
    res -> length = 10;
    res -> bomb_number = 0; 
    res -> safe_first_click = true;
    res -> seed = 0;
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i],"-h") == 0 )
        {   
            res -> status = 1;
            return res;
        }
        switch (i)
        {
        case 1:// length
            int l = atoi(argv[i]);
            if (l <= 0)
                res -> status = -1;
            else 
                res -> length = l;
            break;
        case 2:// width
            int w = atoi(argv[i]);
            if (w <= 0)
                res -> status = -2;
            else 
                res -> width = w;
            break; 
        case 3:// bomb number
            int n = atoi(argv[i]);
            if (n <= 0)
                res -> status = -3;
            else 
                res -> bomb_number = n;
            break;
        case 4:
            int s = atoi(argv[i]);
            if (s <0)
                res -> status = -4;
            else
                res -> seed = s;

            break;
        case 5:// safe click ?
            if (strcmp(argv[i],"F") == 0 || strcmp(argv[i],"f") == 0 || \
                strcmp(argv[i],"false") == 0 || strcmp(argv[i],"False") == 0 || \
                strcmp(argv[i],"FALSE") == 0)
                res-> safe_first_click = false;
        default:
        }
        i++;
    }
    if (res->bomb_number == 0)
        res -> bomb_number = (res -> length * res -> width)/5;
    return res;
}


void update_cell(struct game *game, int x , int y)
{
    if (x < 0 || y < 0 || (size_t)x >=game->length || (size_t)y >= game->width)
        return ;
    int current = game -> board[y * game->length + x] -> bomb_around;
    game -> board[y * game->length + x] -> bomb_around = current+1;
}

void update_neighbors(struct game *game)
{
    int length = game -> length;
    int width = game -> width;
    struct cell **board = game -> board;
    for (int x = 0 ; x < length; x++)
    {
        for (int y = 0; y < width; y++)
        {
            if (board[y*length+x]-> is_bomb ==1)
            {
                update_cell(game,x-1,y-1);
                update_cell(game,x-1,y);
                update_cell(game,x-1,y+1);
                update_cell(game,x,y-1);
                update_cell(game,x,y+1);
                update_cell(game,x+1,y-1);
                update_cell(game,x+1,y);
                update_cell(game,x+1,y+1);
            }
        }
    }

}

struct cell **shuffle_list(struct game *game)
{
    struct cell **result;
    int n = 0;
    int length = game -> length;
    int width = game -> width;
    struct cell **board = game -> board;
    for (int i = 0; i < length * width; i++)
    {
        if (board[i] -> state != SHOWN)
            n++;
    }
    result = calloc(n,sizeof(struct cell*));
    int res_ind = 0;
    for (int i = 0; i < length * width; i++)
    {
        if (board[i] -> state != SHOWN)
        {
            result[res_ind] = board[i];
            res_ind++;
        }
    }
    srand(game->seed);
    for (int i = 0 ; i< n-1; i++)
    {
        int r = rand();
        //while (r%(n -(i+1))+i+1>=n)
            //r = rand();
        int index = r%(n-(i+1))+i+1;
        struct cell *tmp = result[i];
        result[i] = result[index];
        result[index] = tmp;
    }


    return result;
}

int put_mines(struct game *game)
{
    // build cell list
    // shuffle list
    // pick n first correct cell
    // put mines in each cell and update his neighbors   
    int bomb_placed = 0;
    int board_length = game->length * game->width;
    int bomb_number = game -> bomb_number;
    struct cell **shufled = shuffle_list(game);
    while (bomb_placed < bomb_number)
    {
        int i = 0;
        while (bomb_placed < bomb_number && i < board_length)
        {
            //if () condition for anti-clustering
            shufled[i] -> is_bomb = 1;
            bomb_placed++;
            i++;
        }
        
    }
    update_neighbors(game);
    return 0;
}


int reveal(struct game *game, int x, int y)
{
    if (x >= 0 && (size_t)x < game->length && y >= 0 && (size_t)y < game->width)
    {
        int index = y * game->length + x;
        struct cell **board = game-> board;
        if (board[index] -> state == HIDDEN)
        {
            int revealed = 1;
            board[index] -> state = SHOWN;
            if (board[index] -> is_bomb)
                return -1;//bomb found
            // propagation
            if (board[index] -> bomb_around == 0 && board[index] -> is_bomb == 0)
            {
                revealed+=reveal(game,x-1,y-1);
                revealed+=reveal(game,x-1,y);
                revealed+=reveal(game,x-1,y+1);
                revealed+=reveal(game,x,y-1);
                revealed+=reveal(game,x,y+1);
                revealed+=reveal(game,x+1,y-1);
                revealed+=reveal(game,x+1,y);
                revealed+=reveal(game,x+1,y+1);
            }
            return revealed;
        }
    }
    return 0;
}
