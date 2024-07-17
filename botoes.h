#ifndef BOTOES_H
#define BOTOES_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>       
#include <sys/mman.h>
#include <unistd.h>  
#define KEY_BASE 0x0
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000

extern int botao0;
extern int botao1;
extern int botao2;
extern int botao3;

extern volatile int *KEY_ptr;
extern int fd_botoes;
extern void *LW_virtual;

int open_botoes_device();

void read_botoes();

void close_botoes_devide();


#endif // DRAW_SCREEN_H
