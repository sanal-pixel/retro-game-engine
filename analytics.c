#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "analytics.h"

static const char *gameTypeToString(GameType t) {
    switch (t) {
        case GAME_SNAKE:       return "Snake";
        case GAME_TIC_TAC_TOE: return "TicTacToe";
        default:               return "Unknown";
    }
}

struct AnalyticsRecord *buildRecordsFromList(const LeaderboardList *lb, int *total) {
    *total = 0;
    if (lb == NULL || lb->head == NULL) return NULL;

    int n = 0;
    for (const ScoreNode *p = lb->head; p != NULL; p = p->next) n++;

    struct AnalyticsRecord *arr = malloc(n * sizeof(struct AnalyticsRecord));
    if (arr == NULL) return NULL;

    int i = 0;
    for (const ScoreNode *p = lb->head; p != NULL; p = p->next, i++) {
        arr[i].id = i + 1;
        strncpy(arr[i].name, p->player_name, MAX_NAME - 1);
        arr[i].name[MAX_NAME - 1] = '\0';
        strncpy(arr[i].game, gameTypeToString(p->game_type), MAX_GAME - 1);
        arr[i].game[MAX_GAME - 1] = '\0';
        arr[i].score = p->score;
    }
    *total = n;
    return arr;
}

static void printRecord(const struct AnalyticsRecord *r) {
    printf("%-5d %-15s %-10s %-5d\n", r->id, r->name, r->game, r->score);
}

void searchByPlayer(const struct AnalyticsRecord players[], int total, const char *name) {
    int found = 0;
    printf("\nRecords matching player '%s':\n", name);
    for (int i = 0; i < total; i++) {
        if (strstr(players[i].name, name) != NULL) {
            printRecord(&players[i]);
            found = 1;
        }
    }
    if (!found) printf("No records found.\n");
}

void searchByGame(const struct AnalyticsRecord players[], int total, const char *game) {
    int found = 0;
    printf("\nRecords matching game '%s':\n", game);
    for (int i = 0; i < total; i++) {
        if (strstr(players[i].game, game) != NULL) {
            printRecord(&players[i]);
            found = 1;
        }
    }
    if (!found) printf("No records found.\n");
}

/* qsort comparator: highest score first (overflow-safe) */
static int compareByScoreDesc(const void *a, const void *b) {
    const struct AnalyticsRecord *ra = a;
    const struct AnalyticsRecord *rb = b;
    return (rb->score > ra->score) - (rb->score < ra->score);
}

void sortLeaderboard(struct AnalyticsRecord players[], int total) {
    if (total > 1)
        qsort(players, total, sizeof(struct AnalyticsRecord), compareByScoreDesc);
}

void printLeaderboard(const struct AnalyticsRecord players[], int total) {
    printf("\nLeaderboard (highest first):\n");
    for (int i = 0; i < total; i++) {
        printf("%d. %-15s %-10s %d\n",
               i + 1, players[i].name, players[i].game, players[i].score);
    }
}

void generateAuditReport(const struct AnalyticsRecord players[], int total) {
    FILE *fp = fopen("audit_report.txt", "w");
    int snakeCount = 0, ticCount = 0;
    int snakeBest = -1, ticBest = -1;   /* -1 = no records */
    int snakeIdx = -1, ticIdx = -1;

    if (fp == NULL) {
        printf("Error: could not create report file.\n");
        return;
    }

    fprintf(fp, "===== HIGH SCORE AUDIT REPORT =====\n\n");
    fprintf(fp, "%-5s %-15s %-10s %-5s\n", "ID", "Name", "Game", "Score");
    fprintf(fp, "----------------------------------------\n");

    for (int i = 0; i < total; i++) {
        fprintf(fp, "%-5d %-15s %-10s %-5d\n",
                players[i].id, players[i].name,
                players[i].game, players[i].score);

        if (strcmp(players[i].game, "Snake") == 0) {
            snakeCount++;
            if (players[i].score > snakeBest) { snakeBest = players[i].score; snakeIdx = i; }
        } else if (strcmp(players[i].game, "TicTacToe") == 0) {
            ticCount++;
            if (players[i].score > ticBest) { ticBest = players[i].score; ticIdx = i; }
        }
    }

    fprintf(fp, "\nTotal records     : %d\n", total);
    fprintf(fp, "Snake records     : %d\n", snakeCount);
    fprintf(fp, "TicTacToe records : %d\n", ticCount);

    fprintf(fp, "\nHighest scores:\n");
    if (snakeIdx >= 0)
        fprintf(fp, "  Snake     : %d (%s)\n", snakeBest, players[snakeIdx].name);
    else
        fprintf(fp, "  Snake     : none\n");
    if (ticIdx >= 0)
        fprintf(fp, "  TicTacToe : %d (%s)\n", ticBest, players[ticIdx].name);
    else
        fprintf(fp, "  TicTacToe : none\n");

    fclose(fp);
    printf("\nAudit report saved to audit_report.txt\n");
}

