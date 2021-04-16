#ifndef MINESWEEPER_GAMEPLAY_H
#define MINESWEEPER_GAMEPLAY_H

#include "setup.h"

void flag_set(Board *board, Coordonates *coords);
void _cell_set_seen(int *non_mine_counter, Cell *seen_cell);
int cell_discover(Game *game, Coordonates *coords);
void cell_discover_empty(Game *game, Coordonates *coords);

Coordonates *coordonates_select();
Coordonates *coordonates_selection_loop(BoardSize size);

void coordonates_free(Coordonates *self);
Action action_select();
Action action_selection_loop();

int action_is_valid(Action self);
int action_execute(Action self, Game *game, Coordonates *coords);
int user_play(Game *game);
int move_is_valid(Action action, Coordonates *coords);
void game_loop();

#endif