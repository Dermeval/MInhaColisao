#ifndef DRAW_SCREEN_H
#define DRAW_SCREEN_H

#include "gpu_lib.h"

extern int screen;

void draw_pause_screen();

void draw_win_screen();

void draw_fase_screen(int number, int repeat);

void draw_restart_screen();

void draw_game_over_screen();

void draw_start_game_screen();

void draw_end_game_screen();

void draw_bg_cenario();

#endif // DRAW_SCREEN_H
