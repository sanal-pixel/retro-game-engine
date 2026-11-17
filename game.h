
#ifndef GAME_H
#define GAME_H

// Which game is being played?
typedef enum {
    GAME_TIC_TAC_TOE,
    GAME_SNAKE
} GameType;

// What is happening in the game?
typedef enum {
    MENU,
    RUNNING,
    PAUSED,
    GAME_OVER
} GameState;

// Stores a position using x and y coordinates.
typedef struct {
    int x;
    int y;
} Position;

// One part of the snake.
typedef struct SnakeNode {
    Position position;
    struct SnakeNode *next;
} SnakeNode;

// Stores information about the current game.
typedef struct {
    GameType type;
    GameState state;

    int score;
    int moves;

    char player_name[50];

    char board[3][3];
    char current_symbol;

    SnakeNode *snake_head;
    Position food;

    int direction;
} GameSession;

// Stores one player's score.
typedef struct ScoreNode {
    char player_name[50];
    GameType game_type;
    int score;

    struct ScoreNode *next;
} ScoreNode;

// Stores the complete leaderboard.
typedef struct {
    ScoreNode *head;
    int total_scores;
} LeaderboardList;

#endif
