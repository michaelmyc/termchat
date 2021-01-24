#ifndef CLIENT_PROCESSED
#define CLIENT_PROCESSED

#include "../settings.h"
#include "helper.h"

#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>

#define KEY_EOF 4
#define KEY_LF 10
#define KEY_CR 13
#define KEY_ESC 27
#define KEY_BKSP 127
#define KEY_DEL 330
#define KEY_CTRL_UP 567
#define KEY_CTRL_DOWN 526
#define KEY_CTRL_LEFT 546
#define KEY_CTRL_RIGHT 561

void safeQuit();
void init_win();
void init();
void update_sep_win(char *user, char *room, char *network, int input_len);
void log_message(Message *msg);
void wclrline(WINDOW *win, int line);
void mvinputcur(int col);

#endif