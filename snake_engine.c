
#include <stdio.h>
#include <stdlib.h>
#include "snake_engine.h"

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20

// Initialize the snake
void init_snake(GameSession *game) {

    game->snake_head = NULL;
    game->score = 0;
    game->moves = 0;

    // Initial direction: Right
    game->direction = 1;

    // Create three initial snake segments
    for (int i = 2; i >= 0; i--) {

        SnakeNode *new_node = malloc(sizeof(SnakeNode));

        if (new_node == NULL) {
            free_snake(game);
            return;
        }

        new_node->position.x = 5 - i;
        new_node->position.y = 5;

        new_node->next = game->snake_head;
        game->snake_head = new_node;
    }

    game->state = RUNNING;

    spawn_food(game);
}


// Move the snake
void move_snake(GameSession *game) {

    if (game->snake_head == NULL || game->state != RUNNING) {
        return;
    }

    // Calculate the new head position
    Position new_position = game->snake_head->position;

    switch (game->direction) {

        case 0:
            new_position.y--;
            break;

        case 1:
            new_position.x++;
            break;

        case 2:
            new_position.y++;
            break;

        case 3:
            new_position.x--;
            break;

        default:
            return;
    }

    // Check whether the snake will eat food
    int eating = (
        new_position.x == game->food.x &&
        new_position.y == game->food.y
    );

    // Check wall collision
    if (new_position.x < 0 ||
        new_position.x >= BOARD_WIDTH ||
        new_position.y < 0 ||
        new_position.y >= BOARD_HEIGHT) {

        game->state = GAME_OVER;
        return;
    }

    // Check collision with the snake's body
    SnakeNode *current = game->snake_head;

    while (current != NULL) {

        // The tail moves away unless the snake eats food
        if (current->next == NULL && !eating) {
            break;
        }

        if (current->position.x == new_position.x &&
            current->position.y == new_position.y) {

            game->state = GAME_OVER;
            return;
        }

        current = current->next;
    }

    // Allocate memory for the new head
    SnakeNode *new_node = malloc(sizeof(SnakeNode));

    if (new_node == NULL) {
        game->state = GAME_OVER;
        return;
    }

    new_node->position = new_position;
    new_node->next = game->snake_head;

    game->snake_head = new_node;

    // If food was not eaten, remove the tail
    if (!eating) {

        SnakeNode *current = game->snake_head;

        while (current->next != NULL &&
               current->next->next != NULL) {

            current = current->next;
        }

        if (current->next != NULL) {
            free(current->next);
            current->next = NULL;
        }
    }

    game->moves++;

    // If food was eaten, increase score and spawn new food
    if (eating) {

        game->score++;

        spawn_food(game);
    }
}


// Spawn food at a random empty position
void spawn_food(GameSession *game) {

    int empty_found = 0;

    for (int y = 0; y < BOARD_HEIGHT && !empty_found; y++) {

        for (int x = 0; x < BOARD_WIDTH && !empty_found; x++) {

            int occupied = 0;

            SnakeNode *current = game->snake_head;

            while (current != NULL) {

                if (current->position.x == x &&
                    current->position.y == y) {

                    occupied = 1;
                    break;
                }

                current = current->next;
            }

            if (!occupied) {
                empty_found = 1;
            }
        }
    }

    // No empty space left
    if (!empty_found) {
        game->state = GAME_OVER;
        return;
    }

    do {

        game->food.x = rand() % BOARD_WIDTH;
        game->food.y = rand() % BOARD_HEIGHT;

        empty_found = 1;

        SnakeNode *current = game->snake_head;

        while (current != NULL) {

            if (current->position.x == game->food.x &&
                current->position.y == game->food.y) {

                empty_found = 0;
                break;
            }

            current = current->next;
        }

    } while (!empty_found);
}


// Check for wall and body collisions
int check_collision(GameSession *game) {

    if (game->snake_head == NULL) {
        return 1;
    }

    Position head = game->snake_head->position;

    // Check wall collision
    if (head.x < 0 || head.x >= BOARD_WIDTH ||
        head.y < 0 || head.y >= BOARD_HEIGHT) {

        return 1;
    }

    // Check collision with the body
    SnakeNode *current = game->snake_head->next;

    while (current != NULL) {

        if (current->position.x == head.x &&
            current->position.y == head.y) {

            return 1;
        }

        current = current->next;
    }

    return 0;
}


// Free all snake nodes
void free_snake(GameSession *game) {

    SnakeNode *current = game->snake_head;

    while (current != NULL) {

        SnakeNode *next_node = current->next;

        free(current);

        current = next_node;
    }

    game->snake_head = NULL;
}
