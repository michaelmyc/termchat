#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>

#define KEY_ESC 27
#define KEY_LF 10
#define KEY_CR 13
#define KEY_BKSP 127
#define KEY_DEL 330

#define CHAR_LIMIT 280
#define PAD_rows 400

static WINDOW *chat_win;
static WINDOW *sep_win;

void intHandler(int dummy) {
    endwin();
    exit(0);
}

int init_win() {
  chat_win = subwin(stdscr, LINES-2, COLS, 0, 0);
  sep_win = subwin(stdscr, 1, COLS, LINES-2, 0);
  wbkgd(sep_win, ' ' | COLOR_PAIR(3));
}

int log_chat(char name[], char message []) {
  wattron(chat_win, A_BOLD);
  wprintw(chat_win, "%s: ", name);
  wattroff(chat_win, A_BOLD);
  wprintw(chat_win, "%s", message);
}

void wclrline(WINDOW *win, int line) {
  wmove(win, line, 0);
  wclrtoeol(win);
}

void mvinputcur(int col) {
  move(LINES - 1, col);
}

int main() {
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
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_RED);
  }

  signal(SIGINT, intHandler);

  init_win();
  char sep_str[16];
  int input_len = 0;
  int curr_cursor = 0;

  for (;;) {
    log_chat("michael", "This is a super long chat message that will never fit in one line. What will you do about it? Huh? Com'on?");

    wclrline(sep_win, 0);
    sprintf(sep_str, "%d / %d", input_len, CHAR_LIMIT);
    wmove(sep_win, 0, COLS - strlen(sep_str) - 1);
    wprintw(sep_win, sep_str);
    wrefresh(sep_win);

    mvinputcur(curr_cursor);
    int ch = getch();
    // wprintw(input_win, "Type any character to see it in bold ");
    if (ch == KEY_RESIZE) {
      getmaxyx(stdscr, LINES, COLS);
    } else if (ch == KEY_ESC) {
      endwin();
      return 0;
    } else if (ch == KEY_LF || ch == KEY_CR) {
      wclrline(stdscr, LINES-1);
    } else if (isprint(ch)) {
      addch(ch);
      curr_cursor++;
      input_len++;
    } else {
      switch (ch)
      {
      case KEY_BKSP:
        if (curr_cursor > 0) {
          mvdelch(LINES-1, curr_cursor - 1);
          input_len--;
          curr_cursor--;
        }
        break;

      case KEY_DEL:
        if (curr_cursor < input_len) {
          delch();
          input_len--;
        }
        break;

      case KEY_UP:
        // scroll up
        break;

      case KEY_DOWN:
        // scroll down
        break;

      case KEY_LEFT:
        if (curr_cursor > 0) {
          curr_cursor--;
          mvinputcur(curr_cursor);
        }
        break;

      case KEY_RIGHT:
        if (curr_cursor < input_len) {
          curr_cursor++;
          mvinputcur(curr_cursor);
        }
        break;

      default:
        break;
      }
    }
    refresh();
  }

  endwin();
  return 0;
}