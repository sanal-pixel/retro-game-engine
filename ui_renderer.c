#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui_renderer.h"

// Displays the main menu.
void ui_print_main_menu(void) {
    printf("\033[1;36m");
    printf("\n");
    printf("========================================\n");
    printf("             RETRO ARCADE               \n");
    printf("========================================\n");

    printf("\033[1;33m");
    printf("\n");
    printf("          SELECT AN OPTION              \n");

    printf("\033[1;32m");
    printf("\n");
    printf("          [1] Play Tic-Tac-Toe          \n");
    printf("          [2] Play Snake                \n");
    printf("          [3] View Leaderboard          \n");
    printf("          [4] Generate Audit Report     \n");

    printf("\033[1;31m");
    printf("          [5] Save & Exit               \n");

    printf("\033[1;36m");
    printf("\n");
    printf("========================================\n");
    printf("\033[0m");
}

// Displays the menu while playing a game.
void ui_print_game_menu(GameType game_type) {
    const char *title = (game_type == GAME_TIC_TAC_TOE) ? "TIC-TAC-TOE MENU" : "SNAKE MENU";
    printf("\n");
    printf("==============================\n");
    printf("       %s\n", title);
    printf("==============================\n");
    printf("1. View Current Board\n");
    printf("2. Make a Move\n");
    printf("3. Pause / Resume Game\n");
    printf("4. Save Game Progress\n");
    printf("5. End Game\n");
    printf("6. Go Back to Main Menu\n");
    printf("==============================\n");
}

// Clears the terminal screen.
void ui_clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

// Waits for the user to press Enter.
void ui_pause(void) {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // Clear extra characters
    }
}

// Gets a valid menu choice from the user.
int ui_get_menu_choice(int min, int max) {
    char input[100];
    char *end;
    long choice;
    while (1) {
        printf("Enter your choice (%d-%d): ", min, max);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }

        // Check whether the entire input was entered.
        if (strchr(input, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                // Clear extra input.
            }
            printf("\033[1;31mInput is too long. Try again.\033[0m\n");
            continue;
        }

        choice = strtol(input, &end, 10);

        // Check whether the input is a valid number.
        if (end == input || (*end != '\n' && *end != '\0')) {
            printf("\033[1;31mPlease enter a valid number.\033[0m\n");
            continue;
        }

        // Check whether the number is within the allowed range.
        if (choice < min || choice > max) {
            printf("\033[1;31mPlease enter a number between %d and %d.\033[0m\n", min, max);
            continue;
        }
        return (int)choice;
    }
}

// Safely reads text entered by the user.
int ui_read_line(const char *message, char *buffer, size_t size) {
    printf("%s", message);
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    char *newline = strchr(buffer, '\n');
    if (newline != NULL) {
        *newline = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    return 1;
}

// Displays the Tic-Tac-Toe board.
void ui_print_ttt_board(const GameSession *game) {
    printf("\nPlayer: %s | Turn: %c | Moves: %d\n\n",
           game->player_name, game->current_symbol, game->moves);
    printf("   1   2   3\n");
    for (int i = 0; i < 3; i++) {
        printf("%d  ", i + 1);
        for (int j = 0; j < 3; j++) {
            char cell = game->board[i][j];
            if (cell == 'X') {
                printf("\033[1;31mX\033[0m");
            } else if (cell == 'O') {
                printf("\033[1;32mO\033[0m");
            } else {
                printf(" ");
            }
            if (j < 2) {
                printf(" | ");
            }
        }
        printf("\n");
        if (i < 2) {
            printf("  ---+---+---\n");
        }
    }
    printf("\n");
}

// Displays the Snake game board.
void ui_print_snake_board(const GameSession *game, int width, int height) {
    printf("\nPlayer: %s | Score: %d | Moves: %d\n\n",
           game->player_name, game->score, game->moves);

    printf("+");
    for (int x = 0; x < width; x++) {
        printf("-");
    }
    printf("+\n");

    for (int y = 0; y < height; y++) {
        printf("|");
        for (int x = 0; x < width; x++) {
            if (x == game->food.x && y == game->food.y) {
                printf("\033[1;31m*\033[0m");
            } else {
                int is_snake = 0;
                const SnakeNode *current = game->snake_head;
                while (current != NULL) {
                    if (current->position.x == x &&
                        current->position.y == y) {
                        if (current == game->snake_head) {
                            printf("\033[1;32m@\033[0m");
                        } else {
                            printf("\033[0;32mo\033[0m");
                        }
                        is_snake = 1;
                        break;
                    }
                    current = current->next;
                }
                if (!is_snake) {
                    printf(" ");
                }
            }
        }
        printf("|\n");
    }

    printf("+");
    for (int x = 0; x < width; x++) {
        printf("-");
    }
    printf("+\n");
    printf("Legend: @ = Head, o = Body, * = Food\n\n");
}

// Displays the formatted leaderboard.
void ui_print_leaderboard(const LeaderboardList *list) {
    printf("\033[1;33m");
    printf("\n====================================================\n");
    printf("                ARCADE LEADERBOARD                  \n");
    printf("====================================================\n");
    printf("\033[0m");

    if (list == NULL || list->head == NULL) {
        printf("No high scores recorded yet.\n\n");
        return;
    }

    printf("%-5s %-22s %-15s %-8s\n", "Rank", "Player Name", "Game", "Score");
    printf("----------------------------------------------------\n");

    const ScoreNode *current = list->head;
    int rank = 1;
    while (current != NULL) {
        const char *gname = (current->game_type == GAME_TIC_TAC_TOE) ? "Tic-Tac-Toe" : "Snake";
        printf("%-5d %-22s %-15s %-8d\n", rank, current->player_name, gname, current->score);
        rank++;
        current = current->next;
    }
    printf("----------------------------------------------------\n");
    printf("Total Records: %d\n\n", list->total_scores);
}