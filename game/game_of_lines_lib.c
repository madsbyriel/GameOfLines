//
// Created by 45513 on 02-11-2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

typedef struct {
    int x;
    int y;
} Vector2D;
typedef enum {
    PLAYER0,
    PLAYER1,
    FREE
} State;

// Ctrl + Shift + Numpad-  to collapse all. Do that.
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
int get_y_cor(const Vector2D *game_size, const State* arena, int input_x);
int check_if_won(const Vector2D *game_size, const State* arena, State state, int input_x, int line_size);
int ai_move(const Vector2D *game_size, const State* arena);

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

    srand(time(NULL));
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

    // This is where the arena is printed, and this is where input is taken. When playing against AI, input for
    // placement is just random number, and the AI is treated just like a normal player.
    print_arena(arena, game_size);
    if (state == PLAYER0){
        printf("Player 0");
    } else if (state == PLAYER1){
        printf("Player 1");
    } else{
        exit(EXIT_FAILURE);
    }
    printf(" please input a coordinate between %d and %d:\n", 0, game_size->x - 1);
    if (!(state == PLAYER1 && ai_bool)) scan_move(game_size, &input_x);
    else input_x = ai_move(game_size, arena);

    // Was placement successful? Is the map still legit (playable)? Has anyone won?
    int success = set_state(game_size, input_x, arena, state);
    int legit = is_board_legit(game_size, arena);
    int has_winner = check_if_won(game_size, arena, state, input_x, line_size);

    // If it has winner, end game with return.
    if (has_winner){
        print_arena(arena, game_size);
        if (state == PLAYER0){
            printf("Payer 0 has won!");
        } else{
            printf("Player1 has won!");
        }
        return;
    } else if (!legit){ // If the arena is no longer playable, end game.
        print_arena(arena, game_size);
        printf("The map is no longer playable.");
        return;
    } else if (success){ // If placement was successful, switch the state to the other player. AI is always PLAYER1.
        if (state == PLAYER0){
            return main_loop(arena, line_size, ai_bool, game_size, PLAYER1);
        } else {
            return main_loop(arena, line_size, ai_bool, game_size, PLAYER0);
        }
    } else{ // In case the placement was invalid, try the whole damn thing again.
        return main_loop(arena, line_size, ai_bool, game_size, state);
    }
}
void print_arena(const State *arena, const Vector2D *game_size) {
    for (int i = 0; i < game_size->y; ++i) {
        for (int j = 0; j < game_size->x; ++j) {
            State state_i = arena[game_size->y * i + j];
            if (state_i == FREE) printf("_");
            else if (state_i == PLAYER0) printf("0");
            else if (state_i == PLAYER1) printf("1");
            else exit(EXIT_FAILURE);
            if (game_size->x - 1 != j) printf(" ");
        }
        printf("\n");
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
int check_if_won(const Vector2D *game_size, const State* arena, State state, int input_x, int line_size){
    // Get y-coordinate of the point from which we are checking:
    int y = get_y_cor(game_size, arena, input_x);
    if (y == -1){
        // Shouldn't be possible.
        exit(EXIT_FAILURE);
    }

    // Check the horizontal first:
    int counter = 0;
    for (int i = input_x; i < game_size->x; ++i) {
        State state_i = arena[game_size->y * y + i];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
    }
    counter = 0;
    for (int i = input_x; i >= 0; --i) {
        State state_i = arena[game_size->y * y + i];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
    }

    // Check vertical:
    counter = 0;
    for (int i = y; i < game_size->y; ++i) {
        State state_i = arena[game_size->y * i + input_x];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
    }
    counter = 0;
    for (int i = y; i >= 0; --i) {
        State state_i = arena[game_size->y * i + input_x];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
    }

    // Check the diagonals:
    counter = 0;
    int x_cor = input_x;
    int y_cor = y;
    while (x_cor < game_size->x && y_cor < game_size->y)
    {
        State state_i = arena[game_size->y * y_cor + x_cor];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
        ++x_cor; ++y_cor;
    }
    counter = 0;
    x_cor = input_x;
    y_cor = y;
    while (x_cor >= 0 && y_cor >= 0)
    {
        State state_i = arena[game_size->y * y_cor + x_cor];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
        --x_cor; --y_cor;
    }
    counter = 0;
    x_cor = input_x;
    y_cor = y;
    while (x_cor < game_size->x && y_cor >= 0)
    {
        State state_i = arena[game_size->y * y_cor + x_cor];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
        ++x_cor; --y_cor;
    }
    counter = 0;
    x_cor = input_x;
    y_cor = y;
    while (x_cor >= 0 && y_cor < game_size->y)
    {
        State state_i = arena[game_size->y * y_cor + x_cor];
        if (state_i == state) ++counter;
        if (counter >= line_size) return 1;
        else if (state != state_i) break;
        --x_cor; ++y_cor;
    }

    return 0;
}
int get_y_cor(const Vector2D *game_size, const State* arena, int input_x){
    for (int i = 0; i < game_size->y; ++i) {
        State state_i = arena[game_size->y * i + input_x];
        if (state_i != FREE){
            return i;
        }
    }
    return -1;
}
int ai_move(const Vector2D *game_size, const State* arena){
    return rand() % game_size->x;
}