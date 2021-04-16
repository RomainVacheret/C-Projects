#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "setup.h"

Cell *cell_init() {
    Cell *cell = (Cell*) calloc(1, sizeof(Cell));
    return cell;
}

void cell_delete(Cell *self) {
    free(self);
}

Board *board_init(BoardSize *size) {
    int i;
    Board *self = (Board *) malloc(sizeof(Board));
    Cell **matrix = malloc(sizeof(Cell *) * size->columns);
    for(i = 0; i < size->columns; i++) {
        matrix[i] = (Cell *) calloc(size->rows, sizeof(Cell));
    }

    self->size = *size;
    self->matrix = matrix;
    return self;
}

void board_delete(Board *self) {
    int i;
    for(i = self->size.columns - 1; i >= 0; i--) {
        free(self->matrix[i]);
    }
    free(self->matrix);
    free(self);
}

void board_display(Board *self) {
    int i, j;
    _column_number_display(self->size.columns);
    _delimiter_display(self->size.columns);
    for(i = 0; i < self->size.rows; i++) {
        printf("%d|", i);
        for(j = 0; j < self->size.columns; j++) {
            printf("%c|", _char_factory(&self->matrix[i][j]));
        }
        printf("\n");
    }
    _delimiter_display(self->size.columns);
}

void board_discover_everything(Board *self) {
    int i, j;
    for(i = 0; i < self->size.rows; i++) {
        for(j = 0; j < self->size.columns; j++) {
            self->matrix[i][j].state = SEEN;
        }
    }
}

char _cell_get_value(Cell *cell) {
    // Digits have an ASCII code of 48 + 0..9
    return cell->value == MINE ? '!' : 48 + cell->value;
}

char _char_factory(Cell *cell) {
    // Defaut if unseen
    char result = ' ';
    if(cell->state == FLAG) {
        result = '$';
    } else if(cell->state == SEEN) {
        result = _cell_get_value(cell);
    }
    return result;
}

void _delimiter_display(int length) {
    // Length mutiplied by 2 because we space each column value to 
    // display the board more clearly
    int i;
    printf(" +");
    for(i = 0; i < length * 2 - 1; i++) {
        printf("-");
    }
    printf("+");
    printf("\n");
}

void _column_number_display(int length) {
    int i;
    printf(" ");
    for(i = 0; i < length; i++) {
        printf(" %d", i);
    }
    printf("\n");
}

BoardSize get_board_size() {
    BoardSize size = {10, 10};
    return size;
}

int get_mine_count(BoardSize *size) {
    return size->rows * size->columns * 0.1;
}

Game *game_init() {
    Game *self = (Game *) malloc(sizeof(Game));
    BoardSize size = get_board_size();
    Board *board = board_init(&size);
    int mine_count = get_mine_count(&size);
    int non_mine_count = size.rows * size.columns - mine_count;
    mines_init(board, mine_count);

    self->board = board;
    self->non_mine_counter = non_mine_count;
    return self;
}

void game_delete(Game *self) {
    board_delete(self->board);
    free(self);
}

int _is_inside_board(BoardSize size, Coordonates *coords) {
    const int row_validity = coords->row >= 0 && coords->row < size.rows;
    const int column_validity = coords->column >= 0 && coords->column < size.columns;
    return row_validity && column_validity;
}

int _is_current_cell(Coordonates *cell_coords, Coordonates *coords) {
    const int same_row = cell_coords->row == coords->row;
    const int same_column = cell_coords->column == coords->column;
    return same_row && same_column;
}

int _is_mine(Board *board, Coordonates *coords) {
    return board->matrix[coords->row][coords->column].value == MINE;
}

void mine_set(Board *board, Coordonates *coords) {
    // -1/+2 to target all surrounding cells
    int i, j;
    for(i = coords->row - 1; i < coords->row + 2; i++) {
        for(j = coords->column - 1; j < coords->column + 2; j++) {
            Coordonates tmp_coords = {i, j};
            if(_is_inside_board(board->size, &tmp_coords)
                && !_is_current_cell(coords, &tmp_coords) 
                && !_is_mine(board, &tmp_coords)) {
                    board->matrix[i][j].value++;
            }
        }
    }
    board->matrix[coords->row][coords->column].value = MINE;
}

void mines_init(Board *board, int mine_count) {
    srand(time(NULL));
    const int length = board->size.rows * board->size.columns;
    int i, arr[length], counter = 0;
    for(i = 0; i < length; i++) {
        arr[i] = i;
    }
    while(mine_count--) {
        int index = rand() % (length - counter);
        int row = arr[index] / board->size.rows;
        int column = arr[index] % board->size.columns;
        Coordonates coords = {row, column};
        mine_set(board, &coords);
        counter++;
        arr[index] = length - counter;
    }
}