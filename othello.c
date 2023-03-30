#include <stdlib.h>

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>


int8_t* board_empty(){
    int8_t* board = calloc(sizeof(char), 64);
    return board;
}



int8_t* board_from_string(char* s){
    int8_t* board = board_empty();
    for (int i = 0; i<64; i++){
        switch (s[i])
        {
            case '.':
                board[i] = 0;
                break;
            case 'O':
                board[i] = 1;
                break;
            case 'X':
                board[i] = -1;
                break;
        }
    }
    return board;
}


char* board_to_string(int8_t* board){
    char* s = malloc(65*sizeof(char));
    s[64] = '\0';
    for (int i=0; i<64; i++){
        switch (board[i])
        {
            case 0:
                s[i] = '.';
                break;
            case 1:
                s[i] = 'O';
                break;
            case -1:
                s[i] = 'X';
                break;
        }
    }
    return s;
}


void board_display(int8_t* board){
    printf("----------\n");
    for (int i = 0; i<8; i++){
        printf("|");
        for (int j = 0; j<8; j++){
            if (1 == board[i*8+j]){
                printf("O");
            } else if (-1 == board[i*8+j]){
                printf("X");
            } else {
                printf(" ");
            }
        }
        printf("|\n");
    }
    printf("----------\n");
}

    
int8_t* board_copy(int8_t* board){
    int8_t* new = malloc(sizeof(int) * 64);
    for (int i = 0; i<64; i++){
        new[i] = board[i];
    }
    return new;
}


int8_t* board_start(){
    int8_t* board = board_empty();
    
    board[3*8+3] = 1;
    board[4*8+4] = 1;
    board[3*8+4] = -1;
    board[4*8+3] = -1;
    
    return board;
}


int8_t* board_play(int8_t* board, int i, int j, int8_t player){
    if (board[i*8+j] != 0){
        return NULL;
    }

    int8_t* new_board;
    
    bool changed = false;
    
    int x,y;
    
    // on itère sur les 8 directions
    for (int dx=-1; dx<2; dx++){
        for (int dy=-1; dy<2; dy++){
            x = i+dx;
            y = j+dy;
            if (dx != 0 || dy != 0){
                // on se promene dans la direction dx dy jusqu'a sortir du plateau ou ne plus marcher sur des pions ennemis
                // les cas dx == 0 ou dy == 0 sont traités séparemment pour éviter quelques tests (surement négligeable)
                if (0 == dy){
                    while (x>=0 && x<8 && board[x*8+y] == -player){
                        x += dx;
                    }
                } else if (0 == dx){
                    while (y>=0 && y<8 && board[x*8+y] == -player){
                        y += dy;
                    }
                } else {
                    while (x>=0 && x<8 && y>=0 && y<8 && board[x*8+y] == -player){
                        x += dx;
                        y += dy;
                    }   
                }
                if (x>=0 && x<8 && y>=0 && y<8 && player == board[x*8+y]){
                 // situation gagnante
                 // on revient sur nos pas et on retourne la rangée
                    x -= dx;
                    y -= dy;
                     
                    while (x != i || y != j){
                        // on ne réalise une copie du plateau que si le coup est valide
                        if (!changed){
                            new_board = board_copy(board);
                            new_board[i*8+j] = player;
                        }
                        changed = true;
                        new_board[x*8+y] = player;
                        x -= dx;
                        y -= dy;
                    }  
                }
            }
        }
    }
     
    if (changed){
        return new_board;
    } else {
        return NULL;
    } 
} 


int8_t** board_next_moves(int8_t* board, int8_t player){
    int8_t** next = malloc(30 *sizeof(int8_t*));
    int index=0;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            next[index] = board_play(board, i, j, player);
            if (next[index] != NULL){
                index++;
            }
        }
    }
    next[index] = NULL;
    
    return next;
}


int score(int8_t* board){
    int score = 0;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            score += board[i*8+j];
        }
    }
    return score;
}


void heuristic_sort(int8_t** moves, int (*h)(int8_t*, int8_t), int8_t player){
    int n = 0;
    for (n=0; moves[n] != NULL; n++);
    int* scores = malloc(n*sizeof(int));
    for (int i = 0; i < n; i++){
        scores[i] = h(moves[i], player);
    }
    int temp;
    int8_t* temp_board;
    for (int i = 0; i < n-1; i++){
        for (int j = i+1; j<n; j++){
            if (scores[j] > scores[i]){
                temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
                temp_board = moves[i];
                moves[i] = moves[j];
                moves[j] = temp_board;
            }
        }
    }
}


int minimax(int8_t* board, int8_t player, int8_t current_player, int (*h)(int8_t*, int8_t), int depth, int alpha, int beta){
    if (depth == 0){
        return h(board, player);
    } else {
        int8_t** next_moves = board_next_moves(board, current_player);

        heuristic_sort(next_moves, h, current_player);

        int n;
        for (n=0; next_moves[n] != NULL; n++);
        // cas ou aucun coup n'est possible
        if (n==0){
            // return current_player * (-2000)
            free(next_moves);
            return minimax(board, player, -player, h, depth-1, alpha, beta);
        }
        int best;
        int score;
        if (player == current_player){
            // cas max
            best = INT_MIN;
            score;


            for (int i=0; i<n; i++){
                score = minimax(next_moves[i], player, -player, h, depth-1, alpha, beta);
                if (score>best){
                    best = score;
                }
                if (score>beta){
                    break;
                }
                if (score>alpha){
                    alpha = score;
                }
            }
        } else {
            // cas min
            best = INT_MAX;
            score;
            for (int i=0; i<n; i++){
                score = minimax(next_moves[i], player, player, h, depth-1, alpha, beta);
                if (score < best){
                    best = score;
                }
                if (score<alpha){
                    break;
                }
                if (score<beta){
                    beta = score;
                }
            }
        }
    for (int i=0; i<n; i++){
        free(next_moves[i]);
    }
    free(next_moves);
    return best;
    }
}


int8_t* best_move(int8_t* board, int8_t player, int (*h)(int8_t*, int8_t), int depth){
    int8_t** next_moves = board_next_moves(board, player);

    // shuffling list of moves to avoid playing in the same corner too much
    int n;
    for (n=0; next_moves[n] != NULL; n++);
    int j;
    int8_t* temp;
    for (int i=0; i<n; i++){
        j = rand()%n;
        temp = next_moves[i];
        next_moves[i] = next_moves[j];
        next_moves[j] = temp;
    }


    
    if (n==0){
        int8_t* res = board_copy(board);
        return res;
    }
    
    
    // finding move with best score 
    int best = -1000;
    int best_i = 0;
    int score;
    
    for (int i=0; next_moves[i] != NULL; i++){
        score = minimax(next_moves[i], player, -player, h, depth-1, INT_MIN, INT_MAX);
        if (score>best){
            best = score;
            best_i = i;
        }
    }


 // copying best move before freeing the other ones
    int8_t* res = board_copy(next_moves[best_i]);
    for (int i=0; i<n; i++){
        free(next_moves[i]);
    }
    free(next_moves);
    
    return res;
}
     

int compare_heuristics(int (*h1)(int8_t*, int8_t), int (*h2)(int8_t*, int8_t), int depth, int n){
    int total = 0;
    int8_t* b;
    int p;
    int s;
    for (int j=0; j<n; j++){
        printf("Game %d out of %d\n", j,n);
        b = board_start();
        p = (j%2)*2-1;
        for (int i = 0; i<64; i++){
            int8_t* next;
            if (1 == p){
                next = best_move(b, p, h1, depth);
            } else {
                next = best_move(b, p, h2, depth);
            }
            //board_display(next);
            free(b);
            b = next;
            p *= -1;
        }
        s = score(b);
        if (s>0){
            total += 1;
        } else if (s<0){
            total -= 1;
        }
    }
    return total;
}   


int heuristic_1(int8_t* board, int8_t player){
    int weights[8][8] = {{64, -8, 8, 8, 8, 8, -8, 64},
         {-8, -8, 1, 1, 1, 1, -8, -8},
         {8, 1, 1, 1, 1, 1, 1, 8},
         {8, 1, 1, 1, 1, 1, 1, 8},
         {8, 1, 1, 1, 1, 1, 1, 8},
         {8, 1, 1, 1, 1, 1, 1, 8},
         {-8, -8, 1, 1, 1, 1, -8, -8},
         {64, -8, 8, 8, 8, 8, -8, 64}};
         
    int score = 0;
    int sum = 0;
    bool is_full = true;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            sum += board[i*8+j];
            score += weights[i][j]*board[i*8+j];
            if (0==board[i*8+j]){
                is_full = false;
            }
        }
    }
    if (is_full){
        if (sum > 0){
            score = sum + 1000;
        } else if (sum < 0){
            score = sum - 1000;
        } else {
            score = 0;
        }
    }
    return score*player;
}   


int heuristic_2(int8_t* board, int8_t player){
    int CORNER_BONUS = 64;
    int EDGE_BONUS = 8;
    int NEXT_TO_CORNER_BONUS = -16;
    
    int score;
    int sum = 0;
    bool is_full = true;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            sum += board[i*8+j];
            if (0==board[i*8+j]){
                is_full = false;
            }
        }
    }
    score = sum; 
    if (is_full){
        if (sum > 0){
            score = sum + 1000;
        } else if (sum < 0){
            score = sum - 1000;
        } else {
            score = 0;
        }
    } else {
        score += board[0*8+0]*CORNER_BONUS;
        score += board[0*8+7]*CORNER_BONUS;
        score += board[7*8+0]*CORNER_BONUS;
        score += board[7*8+7]*CORNER_BONUS;
        if (0 == board[0*8+0]){
            score += board[0*8+1]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+0]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+1]*NEXT_TO_CORNER_BONUS;
        }
        if (0 == board[0*8+7]){
            score += board[0*8+6]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+6]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+7]*NEXT_TO_CORNER_BONUS;
        }
        if (0 == board[7*8+0]){
            score += board[6*8+0]*NEXT_TO_CORNER_BONUS;
            score += board[6*8+1]*NEXT_TO_CORNER_BONUS;
            score += board[7*8+1]*NEXT_TO_CORNER_BONUS;
        }
        if (0 == board[7*8+7]){
            score += board[6*8+7]*NEXT_TO_CORNER_BONUS;
            score += board[6*8+6]*NEXT_TO_CORNER_BONUS;
            score += board[7*8+6]*NEXT_TO_CORNER_BONUS;
        }
        for (int i=1; i<7; i++){
            score += board[0*8+i] * EDGE_BONUS;
            score += board[i*8+0] * EDGE_BONUS;
            score += board[7*8+i] * EDGE_BONUS;
            score += board[i*8+7] * EDGE_BONUS;
        }   
    }
    return score*player;
}


int heuristic_3(int8_t* board, int8_t player){
    int CORNER_BONUS = 64;
    int EDGE_BONUS = 16;
    int NEXT_TO_CORNER_BONUS = -16;

    int score;
    int sum = 0;
    bool is_full = true;
    for (int i = 0; i<8; i++){
        for (int j = 0; j<8; j++){
            sum += board[i*8+j];
            if (0==board[i*8+j]){
                is_full = false;
            }
        }
    }
    score = sum; 
    if (is_full){
        if (sum > 0){
            score = sum + 1000;
        } else if (sum < 0){
            score = sum - 1000;
        } else {
            score = 0;
        }
    } else {
        // nw corner
        if (board[0] != 0){
            score += CORNER_BONUS * board[0];
            for (int i = 1; i<7 && board[i] == board[0]; i++){score += board[0] * EDGE_BONUS;}
            for (int i = 1; i<7 && board[8*i] == board[0]; i++){score += board[0] * EDGE_BONUS;}
        }
        // ne corner
        if (board[7] != 0){
            score += CORNER_BONUS * board[7];
            for (int i = 1; i<7 && board[7-i] == board[7]; i++){score += board[7] * EDGE_BONUS;}
            for (int i = 1; i<7 && board[8*i+7] == board[7]; i++){score += board[7] * EDGE_BONUS;}
        }
        // sw corner
        if (board[56] != 0){
            score += CORNER_BONUS * board[56];
            for (int i = 1; i<7 && board[8 * (7-i)] == board[56]; i++){score += board[56] * EDGE_BONUS;}
            for (int i = 1; i<7 && board[56+i] == board[56]; i++){score += board[56] * EDGE_BONUS;}
        }
        // se corner
        if (board[63] != 0){
            score += CORNER_BONUS * board[63];
            for (int i = 1; i<7 && board[63 - i] == board[63]; i++){score += board[63] * EDGE_BONUS;}
            for (int i = 1; i<7 && board[63 - 8*i] == board[63]; i++){score += board[63] * EDGE_BONUS;}
        }
        if (0 == board[0*8+0]){
            score += board[0*8+1]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+0]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+1]*NEXT_TO_CORNER_BONUS;
        }
        if (0 == board[0*8+7]){
            score += board[0*8+6]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+6]*NEXT_TO_CORNER_BONUS;
            score += board[1*8+7]*NEXT_TO_CORNER_BONUS;
        }
        if (0 == board[7*8+0]){
            score += board[6*8+0]*NEXT_TO_CORNER_BONUS;
            score += board[6*8+1]*NEXT_TO_CORNER_BONUS;
            score += board[7*8+1]*NEXT_TO_CORNER_BONUS;
        }
        if (0 == board[7*8+7]){
            score += board[6*8+7]*NEXT_TO_CORNER_BONUS;
            score += board[6*8+6]*NEXT_TO_CORNER_BONUS;
            score += board[7*8+6]*NEXT_TO_CORNER_BONUS;
        }
    }
    return score * player;
}


int move_played(int8_t* b1, int8_t* b2){
    for (int i=0; i < 64; i++){
        if (0 == b1[i] && 0 != b2[i]){
            return i;
        }
    }
    return -1;
}


int empty_squares(int8_t* board){
    int n = 0;
    for (int i=0; i<64; i++){
        if (0 == board[i]){
            n++;
        }
    }
    return n;
}




/*
int main(){
    srand(time(NULL));
         
    //printf("%d", compare_heuristics(heuristic_3, heuristic_1, 6, 10));


    int8_t* b = board_start();
    board_display(b);
    int p = -1;
    for (int i = 0; i<64; i++){
        int8_t* next;
        if (1 == p){
            next = best_move(b, p, heuristic_3, 6);
        } else {
            next = best_move(b, p, heuristic_1, 6);
        }
        board_display(next);
        free(b);
        b = next;
        p *= -1;
    }
    printf("%d", score(b));
    

    return 0;
}
*/
