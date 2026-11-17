#include <stdio.h>
#include "game.h"
#include "tictactoe_engine.h"

void ttt_initialize(GameSession *game)
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            game->ttt_board[i][j] = ' ';
        }
    }

    game->current_player_symbol = 'X';
    game->moves_count = 0;
    game->state = GAME_STATE_RUNNING;
    game->type = GAME_TIC_TAC_TOE;
}

/*  fully raw, visulizatioan of code, Like how it look, kind of layout, and just put some numbers inside it

#include<stdio.h>



int main(){
 
        printf("    |    |     \n");
        printf(" 1  | 2  | 3   \n");
        printf("____|____|____ \n");
        printf("    |    |     \n");
        printf(" 4  | 5  | 6   \n");
        printf("____|____|____ \n");
        printf("    |    |     \n");
        printf(" 7  | 8  | 9   \n");
        printf("    |    |     \n");


   
   
    




    return 0;
} */
