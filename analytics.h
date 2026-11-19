#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "file_io.h"   /* pulls in game.h: LeaderboardList, ScoreNode, GameType */

#define MAX_NAME 50    /* matches player_name[50] in game.h */
#define MAX_GAME 20

/* Flat copy of one leaderboard entry, used for search/sort/report */
struct AnalyticsRecord {
    int  id;
    char name[MAX_NAME];
    char game[MAX_GAME];   /* "Snake" or "TicTacToe" */
    int  score;
};

/* Copies the linked list into a new array.
   Returns NULL if the list is empty or malloc fails. *total = count.
   Caller must free() the returned array. */
struct AnalyticsRecord *buildRecordsFromList(const LeaderboardList *lb, int *total);

/* Print records whose player name contains 'name' */
void searchByPlayer(const struct AnalyticsRecord players[], int total, const char *name);

/* Print records whose game contains 'game' */
void searchByGame(const struct AnalyticsRecord players[], int total, const char *game);

/* Sort array in place, highest score first */
void sortLeaderboard(struct AnalyticsRecord players[], int total);

/* Print numbered leaderboard (call sortLeaderboard first) */
void printLeaderboard(const struct AnalyticsRecord players[], int total);

/* Write all records, per-game counts and top scores to audit_report.txt */
void generateAuditReport(const struct AnalyticsRecord players[], int total);

#endif

