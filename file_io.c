
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"

// Temporary structure for storing player information
typedef struct {
    char player_name[50];
    GameType game_type;
    int score;
} ScoreRecord;


// Save leaderboard records into a file
void save_scores_to_file(LeaderboardList *leaderboard)
{
    FILE *fp;

    fp = fopen("leaderboard.dat", "wb");

    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    ScoreNode *current = leaderboard->head;

    while (current != NULL)
    {
        ScoreRecord record;

        strcpy(record.player_name, current->player_name);
        record.game_type = current->game_type;
        record.score = current->score;

        fwrite(&record, sizeof(ScoreRecord), 1, fp);

        current = current->next;
    }

    fclose(fp);

    printf("Scores saved successfully!\n");
}


// Load leaderboard records from a file
void load_scores_from_file(LeaderboardList *leaderboard)
{
    FILE *fp;

    fp = fopen("leaderboard.dat", "rb");

    if (fp == NULL)
    {
        printf("No previous scores found.\n");
        return;
    }

    ScoreRecord record;

    while (fread(&record, sizeof(ScoreRecord), 1, fp) == 1)
    {
        ScoreNode *new_node;

        new_node = malloc(sizeof(ScoreNode));

        if (new_node == NULL)
        {
            printf("Memory allocation failed!\n");
            break;
        }

        strcpy(new_node->player_name, record.player_name);
        new_node->game_type = record.game_type;
        new_node->score = record.score;

        new_node->next = NULL;

        if (leaderboard->head == NULL)
        {
            leaderboard->head = new_node;
        }
        else
        {
            ScoreNode *current = leaderboard->head;

            while (current->next != NULL)
            {
                current = current->next;
            }

            current->next = new_node;
        }

        leaderboard->total_scores++;
    }

    fclose(fp);

    printf("Scores loaded successfully!\n");
}
