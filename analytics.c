#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "analytics.h"

static void printRecord(const struct ScoreRecord *r) {
    printf("%-5d %-15s %-10s %-5d\n", r->id, r->name, r->game, r->score);
}

void searchByPlayer(const struct ScoreRecord players[], int total, const char *name) {
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

void searchByGame(const struct ScoreRecord players[], int total, const char *game) {
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

/* comparator for qsort: highest score first */
static int compareByScoreDesc(const void *a, const void *b) {
    const struct ScoreRecord *ra = a;
    const struct ScoreRecord *rb = b;
    return rb->score - ra->score;
}

void sortLeaderboard(struct ScoreRecord players[], int total) {
    qsort(players, total, sizeof(struct ScoreRecord), compareByScoreDesc);
}

void printLeaderboard(const struct ScoreRecord players[], int total) {
    printf("\nLeaderboard (highest first):\n");
    for (int i = 0; i < total; i++) {
        printf("%d. %-15s %-10s %d\n",
               i + 1, players[i].name, players[i].game, players[i].score);
    }
}

void generateAuditReport(const struct ScoreRecord players[], int total) {
    FILE *fp = fopen("audit_report.txt", "w");
    int snakeCount = 0, ticCount = 0;

    if (fp == NULL) {
        printf("Error: could not create report file.\n");
        return;
    }

    fprintf(fp, "===== AUDIT REPORT =====\n\n");
    fprintf(fp, "%-5s %-15s %-10s %-5s\n", "ID", "Name", "Game", "Score");
    fprintf(fp, "----------------------------------------\n");

    for (int i = 0; i < total; i++) {
        fprintf(fp, "%-5d %-15s %-10s %-5d\n",
                players[i].id, players[i].name,
                players[i].game, players[i].score);

        if (strcmp(players[i].game, "Snake") == 0)
            snakeCount++;
        else if (strcmp(players[i].game, "TicTacToe") == 0)
            ticCount++;
    }

    fprintf(fp, "\nTotal records     : %d\n", total);
    fprintf(fp, "Snake records     : %d\n", snakeCount);
    fprintf(fp, "TicTacToe records : %d\n", ticCount);

    fclose(fp);
    printf("\nAudit report saved to audit_report.txt\n");
}