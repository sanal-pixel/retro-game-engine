///This is My header file, .h
//It tells the other files which functions my Tic tac toe engine provides.
#ifndef TICTACTOE_ENGINE_H
#define TICTACTOE_ENGINE_H

#include "game.h"

void ttt_initialize(GameSession *game);

int ttt_make_move(GameSession *game, int row, int col);

void ttt_switch_turn(GameSession *game);

char ttt_check_winner(const GameSession *game);

int ttt_computer_move(GameSession *game);

#endif
