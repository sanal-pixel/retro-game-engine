
#include <stdio.h>
#include "game.h"
#include "tictactoe_engine.h"

// Starting the fresh Tic-Tac-Toe game
void ttt_initialize(GameSession *game)
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            game->board[i][j] = ' ';
        }
    }

    game->current_symbol = 'X';
    game->moves = 0;
    game->state = RUNNING;
    game->type = GAME_TIC_TAC_TOE;
}

// must change the turn after a player 1 valid move
void ttt_switch_turn(GameSession *game)
{
    if (game->current_symbol == 'X')
        game->current_symbol = 'O';
    else
        game->current_symbol = 'X';
}

// Checking if somebody won or if the board is full
char ttt_check_winner(const GameSession *game)
{
    int i;

    // Checking rows and columns
    for (i = 0; i < 3; i++)
    {
        if (game->board[i][0] != ' ' &&
            game->board[i][0] == game->board[i][1] &&
            game->board[i][1] == game->board[i][2])
            return game->board[i][0];

        if (game->board[0][i] != ' ' &&
            game->board[0][i] == game->board[1][i] &&
            game->board[1][i] == game->board[2][i])
            return game->board[0][i];
    }

    // Check 2 diagonals
    if (game->board[0][0] != ' ' &&
        game->board[0][0] == game->board[1][1] &&
        game->board[1][1] == game->board[2][2])
        return game->board[0][0];

    if (game->board[0][2] != ' ' &&
        game->board[0][2] == game->board[1][1] &&
        game->board[1][1] == game->board[2][0])
        return game->board[0][2];

    // there is No winner? , check if the board is full
    for (i = 0; i < 3; i++)
    {
        int j;

        for (j = 0; j < 3; j++)
        {
            if (game->board[i][j] == ' ')
                return '\0';
        }
    }

    return 'D';
}

// Place current player's symbole
int ttt_make_move(GameSession *game, int row, int col)
{
    char result;

    if (game == NULL || game->state != RUNNING)
        return 0;

    // Check board boundaries
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
        return 0;

    // Don't cut an existing move
    if (game->board[row][col] != ' ')
        return 0;

    game->board[row][col] = game->current_symbol;
    game->moves++;

    result = ttt_check_winner(game);

    if (result != '\0')
        game->state = GAME_OVER;
    else
        ttt_switch_turn(game);

    return 1;
}

// Computer use kind of Pattern recognition algorithms.
//chooses the first available empty cell
int ttt_computer_move(GameSession *game)
{
    int i, j;

    if (game == NULL || game->state != RUNNING)
        return 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (game->board[i][j] == ' ')
                return ttt_make_move(game, i, j);
        }
    }

    return 0;
}
