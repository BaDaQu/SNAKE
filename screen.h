#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <ncurses.h>

void init_screen(); // init ncurses screen
void done_screen(); // end win
void update_screen(); // update screen border

int ngetch(); // get single char

void getscreenanchor(int &y, int &x); // get topleft coords
void getscreensize(int &y, int &x); // get terminal size

int gotoyx(int y, int x); // move cursor
void getcursor(int& y, int& x); // get cursor position

void printc(char c); // print single char to cursor position
int printl(const char* fmt, ...); // printf for terminal (?)

#endif
