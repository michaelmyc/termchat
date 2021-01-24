#ifndef HELPER_PROCESSED
#define HELPER_PROCESSED

#include "../settings.h"

#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

extern Message *chat_log[];
extern int curr_log;
extern pthread_mutex_t *chat_log_mutex;

void get_input_str(char output[], char input[], int len, int start);
int get_win_start(int len, int abs_pos, int prev_start);
Message *create_msg(char *user, char *room, char *network, char *message);
void add_message(Message *msg);

#endif