
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"

// Stores leaderboard information
typedef struct {
    char player_name[50];
    GameType game_type;
    int score;
} ScoreRecord;

// Stores game information
typedef struct {
    char player_name[50];
    GameType type;
    GameState state;
    int score;
    int moves;
    char board[3][3];
    char current_symbol;
    Position food;
    int direction;
    int snake_length;
} GameRecord;


// Save leaderboard scores
void save_scores_to_file(LeaderboardList *leaderboard)
{
    FILE *fp = fopen("leaderboard.dat", "wb");

    if (fp == NULL)
        return;

    ScoreNode *current = leaderboard->head;

    while (current != NULL)
    {
        ScoreRecord record = {0};

        strcpy(record.player_name, current->player_name);
        record.game_type = current->game_type;
        record.score = current->score;

        fwrite(&record, sizeof(record), 1, fp);

        current = current->next;
    }

    fclose(fp);
}


// Load leaderboard scores
void load_scores_from_file(LeaderboardList *leaderboard)
{
    FILE *fp = fopen("leaderboard.dat", "rb");

    if (fp == NULL)
        return;

    ScoreRecord record;

    while (fread(&record, sizeof(record), 1, fp) == 1)
    {
        ScoreNode *new_node = malloc(sizeof(ScoreNode));

        if (new_node == NULL)
            break;

        strcpy(new_node->player_name, record.player_name);
        new_node->game_type = record.game_type;
        new_node->score = record.score;
        new_node->next = NULL;

        if (leaderboard->head == NULL)
            leaderboard->head = new_node;
        else
        {
            ScoreNode *current = leaderboard->head;

            while (current->next != NULL)
                current = current->next;

            current->next = new_node;
        }

        leaderboard->total_scores++;
    }

    fclose(fp);
}


// Save current game
int save_game_state(GameSession *session)
{
    FILE *fp = fopen("game_save.dat", "wb");

    if (fp == NULL)
        return 0;

    GameRecord record = {0};

    strcpy(record.player_name, session->player_name);
    record.type = session->type;
    record.state = session->state;
    record.score = session->score;
    record.moves = session->moves;
    memcpy(record.board, session->board, sizeof(record.board));
    record.current_symbol = session->current_symbol;
    record.food = session->food;
    record.direction = session->direction;

    SnakeNode *current = session->snake_head;

    while (current != NULL)
    {
        record.snake_length++;
        current = current->next;
    }

    if (fwrite(&record, sizeof(record), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    current = session->snake_head;

    while (current != NULL)
    {
        if (fwrite(&current->position, sizeof(Position), 1, fp) != 1)
        {
            fclose(fp);
            return 0;
        }

        current = current->next;
    }

    fclose(fp);
    return 1;
}


// Free Snake nodes
static void free_snake(SnakeNode *head)
{
    while (head != NULL)
    {
        SnakeNode *temp = head;
        head = head->next;
        free(temp);
    }
}


// Load saved game
int load_game_state(GameSession *session)
{
    FILE *fp = fopen("game_save.dat", "rb");

    if (fp == NULL)
        return 0;

    GameRecord record;

    if (fread(&record, sizeof(record), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    if (record.snake_length < 0 || record.snake_length > 1000)
    {
        fclose(fp);
        return 0;
    }

    memset(session, 0, sizeof(GameSession));

    strcpy(session->player_name, record.player_name);
    session->type = record.type;
    session->state = record.state;
    session->score = record.score;
    session->moves = record.moves;
    memcpy(session->board, record.board, sizeof(session->board));
    session->current_symbol = record.current_symbol;
    session->food = record.food;
    session->direction = record.direction;

    SnakeNode *current = NULL;

    for (int i = 0; i < record.snake_length; i++)
    {
        SnakeNode *new_node = malloc(sizeof(SnakeNode));

        if (new_node == NULL)
        {
            free_snake(session->snake_head);
            session->snake_head = NULL;
            fclose(fp);
            return 0;
        }

        if (fread(&new_node->position, sizeof(Position), 1, fp) != 1)
        {
            free(new_node);
            free_snake(session->snake_head);
            session->snake_head = NULL;
            fclose(fp);
            return 0;
        }

        new_node->next = NULL;

        if (session->snake_head == NULL)
            session->snake_head = new_node;
        else
            current->next = new_node;

        current = new_node;
    }

    fclose(fp);
    return 1;
}


// Free leaderboard memory
void free_leaderboard(LeaderboardList *leaderboard)
{
    ScoreNode *current = leaderboard->head;

    while (current != NULL)
    {
        ScoreNode *temp = current;
        current = current->next;
        free(temp);
    }

    leaderboard->head = NULL;
    leaderboard->total_scores = 0;
}
