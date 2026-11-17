#ifndef FILE_IO_H
#define FILE_IO_H

#include "game.h"

void load_scores_from_file(LeaderboardList *leaderboard);
void save_scores_to_file(LeaderboardList *leaderboard);
#endif // FILE_IO_H 