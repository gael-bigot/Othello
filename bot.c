#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "othello.h"


int main(int argc, char* argv[]){
    
    int8_t* board = board_from_string(argv[2]);

    

    int8_t player;

    
    if (0 == strcmp(argv[3], "O")){
        player = 1;
    } else if (0 == strcmp(argv[3], "X")){
        player = -1;
    } else {
        fprintf(stderr, "Player parameter should be O or #");
        return -1;
    }


    int int_param = strtol(argv[4], NULL, 10);


    int8_t* new_board;

    if (0 == strcmp(argv[1], "-check")){
        new_board = board_play(board, int_param / 8, int_param % 8, player);
        if (new_board == NULL){
            fprintf(stdout, "invalid_move");
        } else {
            fprintf(stdout, board_to_string(new_board));
        }
    } else if (0 == strcmp(argv[1], "-play")){
        new_board = best_move(board, player, heuristic_3, int_param);
        int move = move_played(board, new_board);
        fprintf(stdout, "%d", move);
    }
    
    return 0;
}