#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define live '1'
#define dead '0'
#define str 25
#define col 80
#define speed_std 1000000
#define speed_very_fast 100000
#define speed_fast 500000
#define speed_slow 2000000
#define speed_very_slow 3000000

void print_field(char field[str][col]);
int scanning_key(long int * time);
int finder(char field[str][col]);

int main() {
  char field[str][col];
  for (int i = 0; i < str; ++i) {
    for (int j = 0; j < col; ++j) {
      field[i][j] = getchar();
    }
    getchar();
  }
  if (freopen("/dev/tty", "r", stdin) != NULL) {
    long int time = speed_std;
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, 1);
    scrollok(stdscr, 1);
    curs_set(0);
    int changes = 1;
    do {
      curs_set(0);
      changes = finder(field);
      print_field(field);
      refresh();
      if (scanning_key(& time) == 1) {
        break;
      }
        usleep(time);
      clear();
    } while (changes);
      endwin();
  }
  return 0;
}

int scanning_key(long int * time) {
  char key = getch();
  int f = 0;
  if (key == '1') {
    * time = speed_very_slow;
  } else if (key == '2') {
    * time = speed_slow;
  } else if (key == '3') {
      * time = speed_std;
  } else if (key == '4') {
        * time = speed_fast;
  } else if (key == '5') {
          * time = speed_very_fast;
  } else if (key == 'q') {
    f = 1;
  }
  return f;
}

void print_field(char field[str][col]) {
  for (int i = 0; i < str; i++) {
    for (int j = 0; j < col; j++) {
      if (field[i][j] == '1') {
        addch('@');
      } else if (field[i][j] == '0') {
        addch(' ');
      }
    }
    printw("\n");
  }
}

int finder(char field[str][col]) {
  char array1[str][col];
  int counter;
  int i, j;
  int changes = 0;
  for (i = 0; i < str; ++i)
    for (j = 0; j < col; ++j)
      array1[i][j] = field[i][j];
  for (i = 0; i < str; ++i) {
    for (j = 0; j < col; ++j) {
      counter = 0;
      field[i][j] = array1[i][j];
      if (array1[(i - 1 + str) % str][(j - 1 + col) % col] == live)
      ++counter;  // сосед слева сверху
      if (array1[(i - 1 + str) % str][j] == live)
        ++counter;  // сосед сверху
      if (array1[(i - 1 + str) % str][(j + 1 + col) % col] == live)
        ++counter;  // сосед сверху справа
      if (array1[i][(j - 1 + col) % col] == live)
        ++counter;  // сосед слева
      if (array1[i][(j + 1 + col) % col] == live)
        ++counter;  // соед справа
      if (array1[(i + 1 + str) % str][(j - 1 + col) % col] == live)
        ++counter;  // сосед слева снизу
      if (array1[(i + 1 + str) % str][j] == live)
        ++counter;  // сосед снизу
      if (array1[(i + 1 + str) % str][(j + 1 + col) % col] == live)
        ++counter;  // сосед справа снизу
      if (array1[i][j] == dead && counter == 3) {
        field[i][j] = live;
        ++changes;
      }
      if (array1[i][j] == live && counter != 2 && counter != 3) {
        field[i][j] = dead;
        ++changes;
      }
    }
  }
  return changes;
}
