
#ifndef FILE_IO_H
#define FILE_IO_H

#include "game.h"

void load_scores_from_file(LeaderboardList *leaderboard);
void save_scores_to_file(LeaderboardList *leaderboard);

int save_game_state(GameSession *session);
int load_game_state(GameSession *session);

void free_leaderboard(LeaderboardList *leaderboard);

#endif
