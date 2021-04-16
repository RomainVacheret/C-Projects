#ifndef MINESWEEPER_SETUP_H
#define MINESWEEPER_SETUP_H

typedef enum State {UNSEEN, SEEN, FLAG} State;
typedef enum Value {EMPTY, ONE, TWO, THREE, FOUR, MINE} Value;
typedef enum Action {DISCOVER_CELL, PLACE_FLAG} Action;

typedef struct Cell {
    State state;
    Value value;
} Cell;

typedef struct BoardSize {
    int rows, columns;
} BoardSize;

typedef struct Board {
    BoardSize size;
    Cell **matrix;
} Board;

typedef struct Game {
    Board *board;
    int non_mine_counter;
} Game;

typedef struct Coordonates {
    int row, column;
} Coordonates;

Cell *cell_init();
void cell_delete();

Board *board_init(BoardSize *size);
void board_delete(Board *self);
void board_display(Board *self);
void board_discover_everything(Board *self);

char _cell_get_value(Cell *cell);
char _char_factory(Cell *cell);
void _delimiter_display(int length);
void _column_number_display(int length);

BoardSize get_board_size();
int get_mine_count(BoardSize *size);
Game *game_init();
void game_delete(Game *self);

int _is_inside_board(BoardSize size, Coordonates *coords);
int _is_current_cell(Coordonates *cell_coords, Coordonates *coords);
int _is_mine(Board *board, Coordonates *coords);
void _array_init(int *arr, int length);
void mine_set(Board *board, Coordonates *coords);
void mines_init(Board *board, int mine_count);

#endif