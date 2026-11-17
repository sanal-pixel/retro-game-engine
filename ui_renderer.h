#ifndef UI_RENDERER_H
#define UI_RENDERER_H
#include "game.h"
#include <stddef.h>
// Displays the main menu.
void ui_print_main_menu(void);
// Displays the game menu.
void ui_print_game_menu(GameType game_type);
// Clears the terminal screen.
void ui_clear_screen(void);
// Waits for the user to press Enter.
void ui_pause(void);
// Gets a valid number from the user.
int ui_get_menu_choice(int min, int max);
// Safely reads text entered by the user.
int ui_read_line(const char *message, char *buffer, size_t size);
// Displays the Tic-Tac-Toe board.
void ui_print_ttt_board(const GameSession *game);
// Displays the Snake game board.
void ui_print_snake_board(const GameSession *game, int width, int height);
// Displays the formatted leaderboard.
void ui_print_leaderboard(const LeaderboardList *list);
#endif