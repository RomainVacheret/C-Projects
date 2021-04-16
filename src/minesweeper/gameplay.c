#include <stdio.h>
#include <stdlib.h>
#include "gameplay.h"

void flag_set(Board *board, Coordonates *coords) {
    board->matrix[coords->row][coords->column].state = FLAG;
}
void _cell_set_seen(int *non_mine_counter, Cell *seen_cell) {
    seen_cell->state = SEEN;
    (*non_mine_counter)--;
}

int cell_discover(Game *game, Coordonates *coords) {
    Board *board = game->board;
    Cell *targeted_cell = &board->matrix[coords->row][coords->column];
    _cell_set_seen(&game->non_mine_counter, targeted_cell);
    if(targeted_cell->value == EMPTY) {
        cell_discover_empty(game, coords);
    }
    return targeted_cell->value == MINE;
}

void cell_discover_empty(Game *game, Coordonates *coords) {
    int i, j;
    Board *board = game->board;
    for(i = coords->row - 1; i < coords->row + 2; i++) {
        for(j = coords->column - 1; j < coords->column + 2; j++) {
            Coordonates tmp_coords = {i, j};
            if(_is_inside_board(board->size, &tmp_coords)) {
                Cell *cell = &board->matrix[i][j];
                if(cell->state == UNSEEN) {
                    _cell_set_seen(&game->non_mine_counter, cell);
                    if(cell->value == EMPTY) {
                        cell_discover_empty(game, &tmp_coords);
                    }
                } 
            }
        }
    }
}

Coordonates *coordonates_select() {
    Coordonates *coords = (Coordonates *) malloc(sizeof(Coordonates));
    printf("You may choose your coordonates.\nEnter the row first, then the column:\n");
    scanf("%d", &coords->row);
    scanf("%d", &coords->column);
    return coords;
}

Coordonates *coordonates_selection_loop(BoardSize size) {
    Coordonates *coords;
    do {
        coords = coordonates_select();
    } while(!_is_inside_board(size, coords));
    return coords;
}

void coordonates_free(Coordonates *self) {
    free(self);
}

Action action_select() {
    Action action;
    printf("You may choose your action.\n0 to discover a cell, 1 to place a flag:\n");
    scanf("%d", &action);
    return action;
}

Action action_selection_loop() {
    Action action;
    do {
        action = action_select();
    }while(!action_is_valid(action));
    return action;
}

int action_is_valid(Action self) {
    return self == DISCOVER_CELL || self == PLACE_FLAG;
}

// Returns 1 if a mine is discoved else 0
int action_execute(Action self, Game *game, Coordonates *coords) {
    if(self == PLACE_FLAG) {
        flag_set(game->board, coords);
        return 0;
    }

    return cell_discover(game, coords);
}

int user_play(Game *game) {
    Action action = action_selection_loop();
    Coordonates *coords = coordonates_selection_loop(game->board->size);
    int is_game_lost = action_execute(action, game, coords);
    coordonates_free(coords);
    return is_game_lost;
}

void game_loop() {
    Game *game = game_init();
    int is_game_lost = 0, is_game_won = 0;

    while(!is_game_lost && !is_game_won) {
        board_display(game->board);
        is_game_lost = user_play(game);
        is_game_won = !game->non_mine_counter ;
    }

    printf("%s\n", is_game_won ? "VICTORY" : "DEFEAT");
    board_discover_everything(game->board);
    board_display(game->board);

    game_delete(game);
    return;
}