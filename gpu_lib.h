/**
 * \file            gpu_lib.h
 * \brief           Header com funções da bibliotecla para uso da GPU
 */

/*
 * Copyright (c) 2024 Pedro Henrique Araujo Almeida, Demerval, Matheus
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
 * Author:          Pedro Henrique ARAUJO ALMEIDA <phaalmeida1\gmail.com>
 *                  Demerval <optional_email\example.com>
 *                  Matheus <optional_email\example.com>
 */

#ifndef GPU_LIB_H
#define GPU_LIB_H


#include <stdint.h>

#define LEFT 0
#define RIGHT 4
#define UP 2
#define DOWN 6
#define UPPER_RIGHT 1
#define UPPER_LEFT 3
#define BOTTOM_LEFT 5
#define BOTTOM_RIGHT 7

#define DEVICE_PATH "/dev/gpu_driver"

extern int fd;      /*Variavel para guardar acesso ao arquivo do kernel*/

/**
 * \brief           Struct usada para sprits moveis.
 */
typedef struct{
uint16_t pos_x;                                      /*!< Armazena a coordenada X do sprite. */
uint16_t pos_y;                                      /*!< Armazena a coordenada Y do sprite. */
uint16_t direction;                                  /*!< Armazena um numero inteiro indicando o ́angulo de movimento. */
uint16_t offset;                                     /*!< Indica o offset de memoria ́utilizado para a escolha do bitmap armazenado no Processa-dor Grafico. */
uint16_t data_register;                              /*!< Indica o registrador do Banco de Registradores no qual as informaçoes do ̃sprite serao armazenadas. */
uint16_t step_x;                                     /*!< Armazena o numero de pixels que o sprite ira se deslocar no eixo X quando o comando de movimentação for utilizado. */
uint16_t step_y;                                     /*!< Armazena o numero de pixels que o sprite ira se deslocar no eixo Y quando o comando de movimentação for utilizado. */
uint16_t enable;                                     /*!< Habilita/Desabilita a impressao do ̃sprite em um determinado momento. */
uint16_t collision;                                  /*!< Informa se o sprite sofreu alguma colisao. */
} Sprite; 

/**
 * \brief              Struct usada para para os sprites fixos, ou seja, nao possuem movimentação durante o jogo.
 */
typedef struct{
uint16_t coord_x;                                    /*!< Armazena a coordenada X do sprite. */
uint16_t coord_y;                                    /*!< Armazena a coordenada Y do sprite. */
uint16_t offset;                                     /*!< Indica o offset de memoria ́utilizado para a escolha do bitmap armazenado no Processa-dor Grafico. */
uint16_t data_register;                              /*!< Indica o registrador do Banco de Registradores no qual as informaçoes do ̃sprite serao armazenadas. */
uint16_t enable;                                     /*!< Habilita/Desabilita a impressao do ̃sprite em um determinado momento. */
} Sprite_Fixed;

extern Sprite_Fixed fixed_sprites_array[5];
extern Sprite sprites_array[5];

uint8_t set_sprite(uint8_t reg, uint16_t x, uint16_t y, uint8_t offset, uint8_t sp);

uint8_t set_poligono( uint16_t address, uint16_t ref_x, uint16_t ref_y, uint8_t size, uint8_t r, uint8_t g, uint8_t b, uint8_t shape);

uint8_t set_background_block( uint8_t column, uint8_t line, uint8_t R, uint8_t G, uint8_t B);

uint8_t set_background_color(uint8_t R, uint8_t G, uint8_t B);

uint8_t collision(Sprite *sp1, Sprite *sp2);

uint8_t set_sprite_pixel_color( uint16_t address, uint8_t R, uint8_t G, uint8_t B);

uint8_t open_gpu_device ();

uint8_t create_fixed_sprite(uint8_t array_position, uint8_t reg, uint16_t x, uint16_t y, uint8_t offset, uint8_t sp );

uint8_t create_sprite(uint8_t array_position, uint8_t reg, uint16_t x, uint16_t y, uint8_t offset, uint8_t sp );

void close_gpu_devide ();

void change_coordinate(Sprite *sp, uint16_t new_x, uint16_t new_y, uint8_t mirror);

void clear_background_blocks();

void clear_poligonos();

void fill_background_blocks (uint8_t line,  uint8_t R, uint8_t G, uint8_t B);

void clear_sprites();

void draw_sprites_anfranserai();

void draw_sprites_PMD();

void draw_mouse();

#endif /* GPU_LIB_H */