#include <time.h>
#include <stdio.h>
#include <string.h>

#include "othello.h"
#include "cJSON.h"
#include "joueur.h"


char HOSTNAME[] = "";
int PORT;


int main(){
    FILE* channel_in;
    FILE* channel_out;

    connect_to_server(HOSTNAME, PORT, &channel_in, &channel_out);
    send_welcome(channel_out, "Gael_0", "partie_0");

    char* buffer = malloc(1024*sizeof(char));
    server_message* msg;

    while (1){
        if (fscanf(buffer, channel_out)){
            msg = parse_message(buffer);
            if (strcomp(msg->status, "joined")){
            }
            if (strcomp(msg->status, "your_turn")){
                signed char* board = board_from_string(msg->board->data);
                
                signed_char* next = best_move(board, )
            }
        }
        sleep(0.1);
    }




}