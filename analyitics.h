#ifndef ANALYTICS_H
#define ANALYTICS_H

struct ScoreRecord {
    int  id;
    char name[30];
    char game[20];   /* "Snake" or "TicTacToe" */
    int  score;
};

void searchByPlayer(const struct ScoreRecord players[], int total, const char *name);
void searchByGame(const struct ScoreRecord players[], int total, const char *game);
void sortLeaderboard(struct ScoreRecord players[], int total);
void printLeaderboard(const struct ScoreRecord players[], int total);
void generateAuditReport(const struct ScoreRecord players[], int total);

#endif