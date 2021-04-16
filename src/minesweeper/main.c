#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include "gameplay.h"


int main() {
    // BoardSize size = {10, 10};
    // Board *board = board_init(&size);
    // // Coordonates c = {1, 1};
    // // Coordonates d = {1, 2};
    // // mine_set(board, &c);
    // // mine_set(board, &d);
    // mines_init(board, 10);
    // board_display2(board);
    // Coordonates coords;
    // scanf("%d %d", &coords.row, &coords.column);
    // // printf("Coucouc \n");
    // cell_discover(board, &coords);
    // // printf("Fin\n");
    // board_display3(board);
    // board_display2(board);
    // board_display(board);
    // board_delete(board);
    game_loop();
    return EXIT_SUCCESS;
}