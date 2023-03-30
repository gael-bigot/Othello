#ifndef __OTHELLO_H
#define __OTHELLO_H

int8_t* board_empty();

int8_t* board_from_string(char* s);

char* board_to_string(int8_t* board);

void board_display(int8_t* board);

int8_t* board_copy(int8_t* board);

int8_t* board_start();

int8_t* board_play(int8_t* board, int i, int j, int8_t player);

int8_t** board_next_moves(int8_t* board, int8_t player);

int score(int8_t* board);

void heuristic_sort(int8_t** moves, int (*h)(int8_t*, int8_t), int8_t player);

int minimax(int8_t* board, int8_t player, int current_player, int (*h)(int8_t*, int8_t), int depth, int alpha, int beta);

int8_t* best_move(int8_t* board, int8_t player, int (*h)(int8_t*, int8_t), int depth);

int heuristic_1(int8_t* board, int8_t player);

int heuristic_2(int8_t* board, int8_t player);

int heuristic_3(int8_t* board, int8_t player);

int move_played(int8_t* b1, int8_t* b2);

int empty_squares(int8_t* board);


#endif