#ifndef DISPLAY_CONTROLER_H
#define DISPLAY_CONTROLER_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000
#define HEX5_BASE 0x10
#define HEX4_BASE 0x20
#define HEX3_BASE 0x30
#define HEX2_BASE 0x40
#define HEX1_BASE 0x50
#define HEX0_BASE 0x60

extern volatile int *HEX0_BASE_ptr, *HEX1_BASE_ptr, *HEX2_BASE_ptr, *HEX3_BASE_ptr, *HEX4_BASE_ptr, *HEX5_BASE_ptr;
extern int fd_display;
extern void *LW_virtual;

int numbers(int c);

int open_display_device();

void write_score (int score);
void write_lifes (int vidas);
void write_start_display();

void write_number_display(int display_digit, int number);

void close_display_device();

#endif // DRAW_SCREEN_H
