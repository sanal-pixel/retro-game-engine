#include "tictactoe_engine.h"

void ttt_initialize(GameSession *game)
{
    if (game == NULL)
    {
        return;
    }

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
    game->score = 0;

    game->type = GAME_TIC_TAC_TOE;
    game->state = GAME_STATE_RUNNING;
}


int ttt_is_valid_move(
    const GameSession *game,
    int row,
    int col
)
{
    if (game == NULL)
    {
        return 0;
    }

    if (row < 0 || row >= 3 ||
        col < 0 || col >= 3)
    {
        return 0;
    }

    if (game->ttt_board[row][col] != ' ')
    {
        return 0;
    }

    return 1;
}


int ttt_make_move(
    GameSession *game,
    int row,
    int col
)
{
    if (game == NULL)
    {
        return 0;
    }

    if (game->state != GAME_STATE_RUNNING)
    {
        return 0;
    }

    if (!ttt_is_valid_move(game, row, col))
    {
        return 0;
    }

    game->ttt_board[row][col] =
        game->current_player_symbol;

    game->moves_count++;

    if (game->current_player_symbol == 'X')
    {
        game->current_player_symbol = 'O';
    }
    else
    {
        game->current_player_symbol = 'X';
    }

    return 1;
}
