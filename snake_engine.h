#ifndef SNAKE_ENGINE_H
#define SNAKE_ENGINE_H

#include "game.h"

// Initialize the snake
void init_snake(GameSession *game);

// Move the snake
void move_snake(GameSession *game);

// Spawn food
void spawn_food(GameSession *game);

// Check for collisions
int check_collision(GameSession *game);

// Free allocated snake memory
void free_snake(GameSession *game);

#endif // SNAKE_ENGINE_H