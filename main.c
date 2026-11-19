#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "ui_renderer.h"
#include "file_io.h"


static void run_game_session(GameType type) {
    GameSession session;
    memset(&session, 0, sizeof(GameSession));

    int loaded = 0;

    printf("\n1. Load Saved Game\n");
    printf("2. Start New Game\n");

    int choice = ui_get_menu_choice(1, 2);

    if (choice == 1) {
        if (load_game_state(&session)) {
            loaded = 1;
            printf("\nSaved Game Loaded Successfully!\n");
            ui_pause();
        } else {
            printf("\nNo saved game found. Starting a new game.\n");
            ui_pause();
        }
    }

    // Sample snake nodes for a new game
    SnakeNode body_node = { {4, 5}, NULL };
    SnakeNode head_node = { {5, 5}, &body_node };

    if (!loaded) {
        session.type = type;
        session.state = RUNNING;
        session.score = 0;
        session.moves = 0;

        ui_read_line("Enter player name: ",
                     session.player_name,
                     sizeof(session.player_name));

        if (type == GAME_TIC_TAC_TOE) {
            session.current_symbol = 'X';

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    session.board[i][j] = ' ';
                }
            }
        } else {
            session.snake_head = &head_node;
            session.food.x = 10;
            session.food.y = 7;
        }
    } else {
        session.state = RUNNING;
    }

    while (session.state != GAME_OVER) {
        ui_clear_screen();

        if (session.type == GAME_TIC_TAC_TOE) {
            ui_print_ttt_board(&session);
        } else {
            ui_print_snake_board(&session, 20, 15);
        }

        ui_print_game_menu(session.type);
        int action = ui_get_menu_choice(1, 6);

        if (action == -1) {
            break;
        } else if (action == 1) {
            ui_pause();
        } else if (action == 2) {
            if (session.state == PAUSED) {
                printf("\nGame is paused! Resume it first.\n");
            } else {
                session.moves++;
                printf("\nMove registered!\n");
            }
            ui_pause();
        } else if (action == 3) {
            if (session.state == RUNNING) {
                session.state = PAUSED;
                printf("\nGame Paused.\n");
            } else if (session.state == PAUSED) {
                session.state = RUNNING;
                printf("\nGame Resumed.\n");
            }
            ui_pause();
        } else if (action == 4) {
            if (save_game_state(&session)) {
                printf("\nGame Progress Saved Successfully!\n");
            } else {
                printf("\nUnable to Save Game!\n");
            }
            ui_pause();
        } else if (action == 5) {
            session.state = GAME_OVER;
            printf("\nGame Over! Final Score: %d\n", session.score);
            ui_pause();
        } else if (action == 6) {
            break;
        }
    }

    // Free memory only when a saved game was loaded
    if (loaded) {
        SnakeNode *current = session.snake_head;

        while (current != NULL) {
            SnakeNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
}


int main(void) {
    // Initialize and load leaderboard
    LeaderboardList leaderboard = { NULL, 0 };

    load_scores_from_file(&leaderboard);

    int choice;

    while (1) {
        ui_clear_screen();
        ui_print_main_menu();

        choice = ui_get_menu_choice(1, 5);

        if (choice == -1) {
            break;
        } else if (choice == 1) {
            run_game_session(GAME_TIC_TAC_TOE);
        } else if (choice == 2) {
            run_game_session(GAME_SNAKE);
        } else if (choice == 3) {
            ui_clear_screen();
            ui_print_leaderboard(&leaderboard);
            ui_pause();
        } else if (choice == 4) {
            printf("\nGenerating End-of-Day Score Audit Report...\n");
            ui_pause();
        } else if (choice == 5) {
            save_scores_to_file(&leaderboard);
            printf("\nSaving and exiting Retro Arcade. Goodbye!\n");
            break;
        }
    }

      free_leaderboard(&leaderboard);

    return 0;
}