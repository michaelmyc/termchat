#include "helper.h"

void get_input_str(char output[], char input[], int len, int start) {
  if (len < COLS) {
    strncpy(output, input, COLS);
  } else if (start > 0 && len == start + COLS - 1) {
    strcpy(output, "< ");
    strncpy(output + 2, input + start + 2, COLS - 2);
    output[COLS] = '\0';
  } else if (start == 0 && len > COLS - 1) {
    strncpy(output, input, COLS - 2);
    output[COLS - 2] = '\0';
    strcat(output, " >\0");
  } else {
    strcpy(output, "< ");
    strncpy(output + 2, input + start + 2, COLS - 4);
    output[COLS - 2] = '\0';
    strcat(output, " >\0");
  }
}

int get_win_start(int len, int abs_pos, int prev_start) {
  if (prev_start > 0 && abs_pos - prev_start < 2) {
    return prev_start - 1;
  }
  if (prev_start > 0 && len < prev_start + COLS - 1) {
    return prev_start - 1;
  }
  if (len > prev_start + COLS - 1 && abs_pos - prev_start > COLS - 3) {
    return prev_start + 1;
  }
  if (abs_pos - prev_start > COLS - 1) {
    return prev_start + 1;
  }
  return prev_start;
}

Message *create_msg(char *user, char *room, char *network, char *message) {
  Message *msg = malloc(sizeof(Message));
  strcpy(msg->user, user);
  strcpy(msg->room, room);
  strcpy(msg->message, message);
  return msg;
}

void add_message(Message *msg) {
  pthread_mutex_lock(chat_log_mutex);
  if (curr_log == LOG_LIMIT) {
    curr_log--;
    free(chat_log[0]);
    memcpy(chat_log, chat_log + 1, sizeof(Message *) * (LOG_LIMIT - 1));
  }
  chat_log[curr_log++] = msg;
  pthread_mutex_unlock(chat_log_mutex);
}