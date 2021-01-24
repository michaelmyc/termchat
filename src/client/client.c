#include "client.h"

static WINDOW *chat_win;
static WINDOW *sep_win;

int curr_log = 0;
int log_changed = 0;
pthread_mutex_t *chat_log_mutex;
Message *chat_log[LOG_LIMIT];
bool chat_log_initialized = FALSE;

void safeQuit() {
  pthread_mutex_destroy(chat_log_mutex);
  for (int i = 0; i < LOG_LIMIT; i++) {
    free(chat_log[i]);
  }
  endwin();
  exit(0);
}

void init() {
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  set_escdelay(0);
  noecho();
  curs_set(TRUE);
  if (has_colors()) {
    use_default_colors();
    start_color();
    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
  }

  init_win();

  pthread_mutex_init(chat_log_mutex, NULL);
}

void init_win() {
  chat_win = subwin(stdscr, LINES-2, COLS, 0, 0);
  sep_win = subwin(stdscr, 1, COLS, LINES-2, 0);
  wbkgd(sep_win, ' ' | COLOR_PAIR(2));
}

void update_sep_win(char *user, char *room, char *network, int input_len) {
  char char_lim_str[16];
  wclear(sep_win);
  sprintf(char_lim_str, "%d / %d", input_len, MSG_LIMIT);
  wattron(sep_win, A_BOLD);
  mvwprintw(sep_win, 0, 0, " %s %s@%s", user, room, network);
  mvwprintw(sep_win, 0, COLS - strlen(char_lim_str) - 1, char_lim_str);
  wattroff(sep_win, A_BOLD);
  wrefresh(sep_win);
}

void log_message(Message *msg) {
  wattron(chat_win, A_BOLD);
  wprintw(chat_win, "%s: ", msg->user);
  wattroff(chat_win, A_BOLD);
  wprintw(chat_win, "%s\n", msg->message);
  wrefresh(chat_win);
}

void wclrline(WINDOW *win, int line) {
  wmove(win, line, 0);
  wclrtoeol(win);
}

void mvinputcur(int col) {
  move(LINES - 1, col);
}

int main(int argc, char *argv[]) {
  // handle arguments
  if (argc != 4) {
    printf("Usage: termchat NETWORK ROOM_ID USER_ID\n");
    exit(1);
  }

  char user[USERNAME_LIMIT];
  char room[ROOMNAME_LIMIT];
  char network[NETWORK_LIMIT];
  strcpy(network, argv[1]);
  strcpy(room, argv[2]);
  strcpy(user, argv[3]);

  // exit on Ctrl + C
  signal(SIGINT, safeQuit);

  init();

  int input_len = 0;
  int abs_cursor = 0;
  int input_cursor = 0;
  int input_win_start = 0;
  int curr_line = 0;
  char input_str[MSG_LIMIT+1] = "";
  char line[MSG_LIMIT];

  // TODO: start chat msg handler in new thread

  for (;;) {
    // TODO: adapt for scrolling
    wclear(chat_win);
    pthread_mutex_lock(chat_log_mutex);
    for (int i = 0; i < curr_log; i++) {
      log_message(chat_log[i]);
    }
    pthread_mutex_unlock(chat_log_mutex);

    update_sep_win(user, room, network, input_len);

    // display input string correctly
    wclrline(stdscr, LINES - 1);
    input_win_start = get_win_start(input_len, abs_cursor, input_win_start);
    get_input_str(line, input_str, input_len, input_win_start);
    mvprintw(LINES - 1, 0, "%s", line);
    input_cursor = abs_cursor - input_win_start;
    mvinputcur(input_cursor);
    refresh();

    int ch = getch();
    if (isprint(ch) && input_len < MSG_LIMIT) {
      strcpy(input_str + abs_cursor + 1, input_str + abs_cursor);
      input_str[abs_cursor] = ch;
      input_str[input_len + 1] = '\0';
      abs_cursor++;
      input_len++;
    } else if (ch == KEY_ESC || ch == KEY_EOF) {
      safeQuit();
    } else if (ch == KEY_LF || ch == KEY_CR) {
      if (input_str[0] != '\0') {
        Message *msg = create_msg(user, room, network, input_str);
        add_message(msg);
        // TODO: implement submit function
        // send_message(msg);
      }
      wclrline(stdscr, LINES-1);
      abs_cursor = 0;
      input_len = 0;
      strcpy(input_str, "");
    } else {
      switch (ch)
      {
      case KEY_RESIZE:
        clear();
        delwin(chat_win);
        delwin(sep_win);
        getmaxyx(stdscr, LINES, COLS);
        init_win();
        break;

      case KEY_BKSP:
        if (abs_cursor > 0) {
          strcpy(input_str + abs_cursor - 1, input_str + abs_cursor);
          input_len--;
          abs_cursor--;
        }
        break;

      case KEY_DEL:
        if (abs_cursor < input_len) {
          char tmp[MSG_LIMIT+1];
          strcpy(tmp, input_str + abs_cursor + 1);
          strcpy(input_str + abs_cursor, tmp);
          input_len--;
        }
        break;

      // TODO: Implement chat scrolling
      // case KEY_UP:
      //   // scroll up
      //   break;

      // case KEY_DOWN:
      //   // scroll down
      //   break;

      case KEY_LEFT:
        if (abs_cursor > 0) {
          abs_cursor--;
        }
        break;

      case KEY_RIGHT:
        if (abs_cursor < input_len) {
          abs_cursor++;
        }
        break;
      
      // TODO: Implement word jumping
      // case KEY_CTRL_LEFT:
      //   break;
      
      // case KEY_CTRL_RIGHT:
      //   break;

      case KEY_HOME:
        abs_cursor = 0;
        break;

      case KEY_END:
        abs_cursor = input_len;
        break;

      default:
        break;
      }
    }
  }

  safeQuit();
  return 0;
}