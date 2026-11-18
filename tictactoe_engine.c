
#include <stdio.h>
#include "game.h"
#include "tictactoe_engine.h"


void ttt_initialize(GameSession *game)
{
    int i, j;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            game->ttt_board[i][j] = ' ';
        }
    }

    game->current_player_symbol = 'X';
    game->moves_count = 0;
    game->state = GAME_STATE_RUNNING;
    game->type = GAME_TIC_TAC_TOE;
}


// display the board with position numbers
void ttt_display_board(GameSession *game)
{
    int i, j;
    int pos = 1;

    printf("\n       TIC TAC TOE\n\n");

    for(i = 0; i < 3; i++)
    {
        printf("       |       |       \n");

        for(j = 0; j < 3; j++)
        {
            if(game->ttt_board[i][j] == ' ')
            {
                printf("   %d   ", pos);
            }
            else
            {
                printf("   %c   ", game->ttt_board[i][j]);
            }

            if(j != 2)
                printf("|");

            pos++;
        }

        printf("\n");

        if(i != 2)
        {
            printf("_______|_______|_______\n");
        }
    }

    printf("       |       |       \n\n");
}
