#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include "mouse_move.h"


int fd_mouse = 0;
uint16_t pos_x = 0;
uint16_t pos_y = 0;
uint8_t left = 0;
uint8_t middle = 0;
uint8_t right = 0;

int open_mouse_device()
{
    fd_mouse = open(MICE_PATH, O_RDWR);

    if (fd_mouse < 0)
    {
        perror("Failed to open the device");
        return 0;
    }
    return 1;
}

void close_mouse_device()
{
    close(fd_mouse);
}

void mouse_movement()
{
    int bytes = read(fd_mouse, data, sizeof(data)); // Abre o arquivo MICE para leitura dos eventos de input que o mouse esta enviando

    left = data[0] & 0x1;   // Lê o 1º LSB e se for igual a 1 significa que o botão esquerdo foi pressionado então left = 1 tambem
    right = data[0] & 0x2;  // Lê o 2º LSB e se for igual a 1 significa que o botão direito foi pressionado então right = 1 tambem
    middle = data[0] & 0x4; // Lê o 4º LSB e se for igual a 1 significa que o botão do meio foi pressionado então middle = 1 tambem

    pos_x += data[1]/2;
    pos_y -= data[2]/2;
    
    if (pos_x < 0)
    {
        //printf("Y: %d\n", pos_y);
        pos_x = 0;
    }
    if (pos_x > 620)
    {
        //printf("MOVE X: %d\n", data[1]);
        pos_x = 0;
    }

    if (pos_y < 0)
    {
        //printf("MOVE Y: %d\n", data[2]);
        pos_y = 0;
    }
    if (pos_y > 460)
    {
        //printf("MOVE Y: %d\n", data[2]);
        pos_y = 460;
    }
    //printf("Y: %d\n", pos_y);
}