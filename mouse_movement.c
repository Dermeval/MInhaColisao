
/**
 * \file            main.c
 * \brief           Programa que quando compilado será executado para formar um desenho no monitor usando a GPU
 */

/*
 * Copyright (c) year FirstName LASTNAME
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of library_name.
 *
 * Author:          FirstName LASTNAME <optional_email@example.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "gpu_lib.h"
#include "mouse_move.h"

int main()
{   
    /* Tentar abrir o arquivo do kernel do driver da GPU */
    if (open_gpu_device() == 0)
        return 0;
    if (open_mouse_device() == 0)
        return 0;

    /* system("clear");
    printf("COMEÇANDO\n");
    while (1) {
        mouse_movement();
        printf("\nCord X: %d, Cord Y: %d, BT_LEF: %d, BT_RIGHT: %d\n", pos_x, pos_y, left, right);
    } */
    set_background_color(0, 0, 0); /* Coloca a cor do background como preto */
    draw_mouse();
    set_sprite(1, 0, 0, 25, 1);
    while (1) {
        mouse_movement();
        set_sprite(1, pos_x, pos_y, 25, 1);
        if (left == 1) {
            printf("\nCABO\n");
            break;
        }
    }



    close_gpu_devide(); /* Fecha o arquivo do driver da GPU */
    close_mouse_device();

    return 0;
}