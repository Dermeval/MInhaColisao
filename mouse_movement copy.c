#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "gpu_lib.h"
#include <stdlib.h>
#include <fcntl.h>       
#include <sys/mman.h>
#include "draw_screens.h" 
#include "draw_sprites.h"
#include "mouse_move.h"

#define KEY_BASE 0x0
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000

int main()
{   
    /* Tentar abrir o arquivo do kernel do driver da GPU */
    if (open_gpu_device() == 0)
        return 0;
    if (open_mouse_device() == 0)
        return 0;

    clear_background_blocks();
    draw_player();
    draw_bomb();

    create_sprite(0, 15, 0, 0, 0, 1); /* SPRITE DO JOGADOR */
    create_sprite(1, 10, 100, 100, 1, 1); /* SPRITE DA BOMBA */
    create_sprite(2, 11, 540, 380, 1, 1); /* SPRITE DA BOMBA */
    create_sprite(3, 12, 100, 380, 1, 1); /* SPRITE DA BOMBA */
    create_sprite(4, 13, 540, 100, 1, 1); /* SPRITE DA BOMBA */
    while (1) {
        //printf("\nX = %d , Y = %d\n", pos_x, pos_y);
        mouse_movement();
    
        change_coordinate(&sprites_array[0], pos_x, pos_y, 0);
    }

    close_gpu_devide(); /* Fecha o arquivo do driver da GPU */
    close_mouse_device();

    return 0;
}