//
// Created by 45513 on 02-11-2022.
//

#include "stdio.h"
#include "stdlib.h"

typedef struct {
    int x;
    int y;
} Vector2D;

typedef enum {
    PLAYER0,
    PLAYER1,
    FREE
} State;
// ----------------------------------------------------------------------------------
// Prototypes:
    // Start the game of lines:
void start_of_game_input(Vector2D* game_size, int* line_size, int* ai_bool);
void get_map_dimensions(Vector2D *game_size);
void get_ai_bool(int *ai_bool);
void get_winning_line_size(int *line_size);
void validate_start_input(Vector2D *game_size, const int *line_size);

    // Dynamically initialize map
State* allocate_arena(Vector2D* game_size);
State* setup_arena(Vector2D *game_size);

    // Main loop:
void main_loop(State* arena, int line_size, int ai_bool, Vector2D* game_size, State state);
void scan_move(const Vector2D *game_size, int *input_x);
void print_arena(const State *arena, const Vector2D *game_size);
int set_state(const Vector2D *game_size, int input_x, State* arena, State state);
int is_board_legit(const Vector2D *game_size, const State* arena);

// ----------------------------------------------------------------------------------
// Definitions:
void game_of_lines(){
    // Start the game of lines:
        // Against AI or player?
        // Game size?
        // Winning line size?
        // Check whether the line size is bigger than the map

    // Dynamically initialize map

    // Main loop:
        // Print arena
        // Get input
            // Input validation
        // Try to place the point
            // If invalid, go get some new input
        // Check if this new move can in any combination be a winning line
            // If winning line, print the player, break
            // Else, get some new input

    int ai_bool = 0;
    int line_size = 0;
    Vector2D game_size = {0, 0};

    // Start the game of lines:
    start_of_game_input(&game_size, &line_size, &ai_bool);

    // Dynamically initialize map
    State* arena = setup_arena(&game_size);

    // Main loop:
    main_loop(arena, line_size, ai_bool, &game_size, PLAYER0);

    free(arena);
}


// Start the game of lines:
void start_of_game_input(Vector2D* game_size, int* line_size, int* ai_bool){
    // Against AI or player?
    get_ai_bool(ai_bool);

    // Game size?
    get_map_dimensions(game_size);

    // Winning line size?
    get_winning_line_size(line_size);

    // Check whether the line size is bigger than the map
    validate_start_input(game_size, line_size);
}
void validate_start_input(Vector2D *game_size, const int *line_size) {
    if (*line_size > game_size->x && *line_size > game_size->y){
        printf("It is impossible to win with these dimensions. Please update them.\n");
        game_size->y = 0;
        game_size->x = 0;
        get_map_dimensions(game_size);
    }
    if (*line_size > game_size->x && *line_size > game_size->y){
        validate_start_input(game_size, line_size);
    }
}
void get_winning_line_size(int *line_size) {
    int i = 0;
    printf("Now enter how long the winning line should be:");
    while (*line_size <= 0){
        if (i != 0) printf("Please, in an integer, above zero:");
        scanf(" %d", line_size);
        ++i;
    }
}
void get_ai_bool(int *ai_bool) {
    char input = '\0';
    printf("Play against computer? (y/n):");
    int i = 0;
    while (input != 'y' && input != 'n'){
        if (i != 0) printf("Please, either 'y' or 'n':");
        fflush(stdin);
        scanf(" %c", &input);
        ++i;
    }

    if (input == 'n'){
        *ai_bool = 0;
    } else{
        *ai_bool = 1;
    }
}
void get_map_dimensions(Vector2D *game_size) {
    int i = 0;
    printf("Now, input the width of the desired map:");
    while (game_size->x <= 0){
        if (i != 0) printf("Please, in the format of an integer, above zero:");
        fflush(stdin);
        scanf(" %d", &game_size->x);
        ++i;
    }

    i = 0;
    printf("Now, input the height of the desired map:");
    while (game_size->y <= 0){
        if (i != 0) printf("Please, in the format of an integer, above zero:");
        fflush(stdin);
        scanf(" %d", &game_size->y);
        ++i;
    }
}

// Dynamically initialize map
State* allocate_arena(Vector2D* game_size){
    return (State*) malloc(sizeof(int) * game_size->x * game_size->y);
}
State* setup_arena(Vector2D *game_size) {
    State* arena = allocate_arena(game_size);
    for (int i = 0; i < (*game_size).x * (*game_size).y; ++i) {
        arena[i] = FREE;
    }
    return arena;
}

// Main loop:
void main_loop(State* arena, int line_size, int ai_bool, Vector2D* game_size, State state){
    int input_x = -1;

    print_arena(arena, game_size);

    if (state == PLAYER0){
        printf("Player 0");
    } else if (state == PLAYER1){
        printf("Player 1");
    } else{
        exit(EXIT_FAILURE);
    }
    printf(" please input a coordinate between %d and %d:", 0, game_size->x - 1);
    scan_move(game_size, &input_x);

    int success = set_state(game_size, input_x, arena, state);
    int legit = is_board_legit(game_size, arena);
    int has_winner = check_if_won(game_size, arena, state);

    if (success){
        if (state == PLAYER0){
            return main_loop(arena, line_size, ai_bool, game_size, PLAYER1);
        } else {
            return main_loop(arena, line_size, ai_bool, game_size, PLAYER0);
        }
    } else{
        return main_loop(arena, line_size, ai_bool, game_size, state);
    }
}
void print_arena(const State *arena, const Vector2D *game_size) {
    int y,
        last_y = 0;
    for (int i = 0; i < game_size->x * game_size->y; ++i) {
        State state_i = arena[i];
        y = i / game_size->y;
        if (y != last_y) printf("\n");
        else if (i != 0) printf(" ");

        if (state_i == FREE) printf("_");
        else if (state_i == PLAYER0) printf("0");
        else if (state_i == PLAYER1) printf("1");
        else exit(EXIT_FAILURE);

        if (game_size->x * game_size->y - 1 == i) printf("\n");

        last_y = y;
    }
}
void scan_move(const Vector2D *game_size, int *input_x) {
    int i = 0;
    while ((*input_x) < 0 || (*input_x) >= game_size->x){
        if (i != 0) printf("Please, input a coordinate between %d and %d:", 0, game_size->x - 1);
        fflush(stdin);
        scanf(" %d", input_x);
        ++i;
    }
}
int set_state(const Vector2D *game_size, int input_x, State* arena, State state){
    for (int i = game_size->y - 1; i >= 0; --i) {
        State* state_i = &arena[i * game_size->y + input_x];
        if (*state_i == FREE){
            *state_i = state;
            return 1;
        } else if (i == 0){
            printf("Invalid choice.\n");
        }
    }
    return 0;
}
int is_board_legit(const Vector2D *game_size, const State* arena){
    for (int i = 0; i < game_size->x; ++i) {
        if (arena[i] == FREE){
            return 1;
        }
    }
    return 0;
}