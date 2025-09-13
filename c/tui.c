#include "curses.h"
#include "stdbool.h"
#include "stdlib.h"
#include "signal.h"

void finish(int signal) {
    endwin();

    switch (signal) {
    case SIGINT:
        puts("Got sigint");
    break;
    case SIGABRT:
        puts("Got sigabrt");
    break;
    case SIGTERM:
        puts("Got sigterm");
    break;
    default:
        printf("Some signal: %i", signal);
    }

    exit(1);
}

int clamp(int v, int min, int max) {
    return (v < min) ? min :
            (v > max) ? max : v;
}

int abs(int v) {
    return v < 0 ? -v : v;
}

int main(int argc, char**argv) {

    signal(SIGINT, finish);
    signal(SIGABRT, finish);
    signal(SIGTERM, finish);

    initscr();
    cbreak();
    noecho();

    for(int y = 0;y < LINES;y += 1) {
        for(int x = 0;x < COLS;x += 1) {
            int c = (abs(y-x) % 26) + 'A';
            mvaddch(y, x, c);
        }

    }

    int cy = 0;
    int cx = 0;
    while (true) {
        int c = getch();

        if (c == 'q') {
            puts("");
            break;
        }

        switch ((char)c) {
        case 'h':
            cx -= 1;
        break;
        case 'j':
            cy += 1;
        break;
        case 'k':
            cy -= 1;
        break;
        case 'l':
            cx += 1;
        break;
        default:
        }

        cx = clamp(cx, 0, COLS - 1);
        cy = clamp(cy, 0, LINES - 1);

        move(cy, cx);

        refresh();
    }

    endwin();

    // printf("sizes: %i %i\n", LINES, COLS);

    return 0;
}