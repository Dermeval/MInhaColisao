/**
 * \file            gpu_lib.c
 * \brief           Biblioteca que implementa as funções do header gpu_lib.h para uso da GPU
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

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include "gpu_lib.h"

int fd = 0;

Sprite_Fixed fixed_sprites_array[5];
Sprite sprites_array[32];


/**
 * \brief           Usada para abrir o arquivo do driver da GPU
 *  \return         Retorna 1 caso o arquivo foi aberto ou retorna 0 caso não seja possivel abrir o arquivo
 */
uint8_t open_gpu_device()
{
    fd = open(DEVICE_PATH, O_WRONLY);

    if (fd < 0)
    {
        perror("Failed to open the device");
        return 0;
    }
    return 1;
}

/**
 * \brief           Usada para fechar o arquivo do driver da GPU
 */
void close_gpu_devide()
{
    close(fd);
}

/**
 * \brief           Usada para configurar a cor base do background a partir dos valores de Red, Green e Blue.
 *
 * \param[in]       R: Valor para a cor vermelha.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       B: Valor para a cor azul.
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
 */
uint8_t set_background_color(uint8_t R, uint8_t B, uint8_t G)
{
    unsigned char command[4];

    command[0] = 0;
    command[1] = R;
    command[2] = G;
    command[3] = B;

    if (write(fd, command, sizeof(command)) < 0)
    {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para setar um sprite na tela
 *
 * \param[in]       reg: Registrador ao qual o sprite será armazenado
 * \param[in]       x: Coordenada x do sprite na tela
 * \param[in]       y: Coordenada y do sprite na tela
 * \param[in]       offset: Deslocamento na memória para selecionar o bitmap
 * \param[in]       sp: Ativação do sprite (0 - desativado, 1 - ativado)
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
 */
uint8_t set_sprite(uint8_t reg, uint16_t x, uint16_t y, uint8_t offset, uint8_t sp)
{
    unsigned char command[7];

    command[0] = 1;
    command[1] = reg;
    command[2] = (offset >> 1) & 0xFF;
    command[3] = ((offset & 0x01) << 7) | ((x >> 3) & 0x7F);
    command[4] = ((x & 0x07) << 5) | ((y >> 5) & 0x1F);
    command[5] = (y & 0x1F) << 3;
    command[6] = sp;

    if (write(fd, command, sizeof(command)) < 0)
    {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para setar um poligono na tela
 *
 * \param[in]       address: Endereço onde o poligono estará armazenado
 * \param[in]       ref_x: Coordenada x na tela referente ao centro do poligono
 * \param[in]       ref_y: Coordenada y na tela referente ao centro do poligono
 * \param[in]       size: Tamanho do poligono
 * \param[in]       r: Valor para a cor vermelha
 * \param[in]       g: Valor para a cor verde
 * \param[in]       b: Valor para a cor azul
 * \param[in]       shape: Formato do poligono (0 = quadrado, 1 = triangulo)
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
 */
uint8_t set_poligono(uint16_t address, uint16_t ref_x, uint16_t ref_y, uint8_t size, uint8_t r, uint8_t g, uint8_t b, uint8_t shape)
{
    unsigned char command[7];

    command[0] = 4;
    command[1] = address;
    command[2] = ref_x >> 1;
    command[3] = ((ref_x & 0b01) << 7) | (ref_y >> 2);
    command[4] = ((ref_y & 0b11) << 6) | (size & 0b1111);
    command[5] = ((r & 0b111) << 5) | (g & 0b111) << 2;
    command[6] = ((b & 0b111) << 5) | shape & 0b1;

    if (write(fd, command, sizeof(command)) < 0)
    {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief Usada para modelar o background atraves do preenchimento dos blocos de 8x8 pixels
 *
 * \param[in]       column: Valor da coluna do bloco.
 * \param[in]       line: Valor da linha do bloco.
 * \param[in]       R: Valor para a cor vermelha.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       B: Valor para a cor azul.
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
 */
uint8_t set_background_block(uint8_t column, uint8_t line, uint8_t R, uint8_t G, uint8_t B)
{
    unsigned char command[5];
    // 0001 1111 1111 1111 000
    int i = 0;
    int address = column + (line * 80);

    command[0] = 2;
    command[1] = (address >> 5);
    command[2] = ((address) << 3) | R;
    command[3] = G;
    command[4] = B;

    // printf("address[1]: %d\n", (R & 0b111));

    if (write(fd, command, sizeof(command)) < 0)
    {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para mudar o valor RGB de um determinado pixel de um sprite com base no endereço de memoria.
 *
 * \param[in]       address: Endereço de memoria do local que deve ter o valor alterado.
 * \param[in]       R: Valor para a cor vermelha.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       B: Valor para a cor azul.
 * \return          Retorna 1 quando colisão foi detectada e 0 quando não.
 */
uint8_t set_sprite_pixel_color(uint16_t address, uint8_t R, uint8_t G, uint8_t B)
{
    unsigned char command[6];

    command[0] = 3;                    // Command for instrucao_wsm
    command[1] = (address >> 6);       // Higher 8 bits of 14-bit address
    command[2] = (address & 0b111111); // Lower 6 bits of address and r
    command[3] = R & 0b111;
    command[4] = G & 0b111; // g value
    command[5] = B & 0b111; // b value

    if (write(fd, command, sizeof(command)) < 0)
    {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para atualizar as coordenadas x e y de um sprit móvel de acordo ao seu ângulo de movimento e valor de deslocamento.
 *
 *
 * \param[in]       sp: Ponteiro do sprite que deve ter suas coordenadas atualizadas
 * \param[in]       mirror: Define se as coordendas do sprite enviado devem ser espelhadas
 */
void change_coordinate(Sprite *sp, uint16_t new_x, uint16_t new_y, uint8_t mirror)
{
    (*sp).pos_x = new_x;
    (*sp).pos_y = new_y;
    set_sprite((*sp).data_register, new_x, new_y, (*sp).offset, (*sp).enable);
}

/**
 * \brief           Usada para verificar se ocorreu uma colisão entre dois sprites quaisquer a partir da técnica de sobreposição de retângulos.
 *
 * \param[in]       sp1: Ponteiro para o sprite 1.
 * \param[in]       sp2: Ponteiro para o sprite 2.
 * \return          Retorna 1 quando colisão foi detectada e 0 quando não.
 */
uint8_t collision(Sprite *sp1, Sprite *sp2)
{

    int square1_right = (*sp1).pos_x + 20;
    int square1_bottom = (*sp1).pos_y - 20;
    int square2_right = (*sp2).pos_x + 20;
    int square2_bottom = (*sp2).pos_y - 20;

    if ((*sp1).pos_x >= square2_right || (*sp2).pos_x >= square1_right)
    {
        (*sp1).collision = 0;
        (*sp2).collision = 0;
        return 0;
    }
    if ((*sp1).pos_y <= square2_bottom || (*sp2).pos_y <= square1_bottom)
    {
        (*sp1).collision = 0;
        (*sp2).collision = 0;
        return 0;
    }
    (*sp1).collision = 1;
    (*sp2).collision = 1;
    return 1;
}

/**
 * \brief           Usada para setar o valor "510" no RGB de todos os background blocks, assim fazendo eles copiar a cor padrão do background
 */
void clear_background_blocks()
{
    int i = 0;
    int j = 0;
    for (i; i < 60; i++)
    {
        for (j; j < 80; j++)
        {
            set_background_block(j, i, 6, 7, 7);
        }
        j = 0;
    }
}

/**
 * \brief           Usada para a partir de uma determinada linha, preencher todos os background blocks que estão na linha e todos os abaixo da linha
 * \param[in]       line: Valor da linha da tela 60x80
 */
void fill_background_blocks(uint8_t line, uint8_t R, uint8_t G, uint8_t B)
{
    int i = line;
    int j = 0;
    for (i; i < 60; i++)
    {
        for (j; j < 80; j++)
        {
            set_background_block(j, i, R, G, B);
        }
        j = 0;
    }
}

uint8_t create_fixed_sprite(uint8_t array_position, uint8_t reg, uint16_t x, uint16_t y, uint8_t offset, uint8_t sp ) {
    fixed_sprites_array[array_position].data_register = reg;
    fixed_sprites_array[array_position].coord_x = x;
    fixed_sprites_array[array_position].coord_y = y;
    fixed_sprites_array[array_position].enable = sp;
    fixed_sprites_array[array_position].offset = offset;

    int ok = set_sprite(reg, x, y, offset, sp);
    return ok;

}

uint8_t create_sprite(uint8_t array_position, uint8_t reg, uint16_t x, uint16_t y, uint8_t offset, uint8_t step_x, uint8_t step_y, uint8_t direction, uint8_t sp ) {
    sprites_array[array_position].data_register = reg;
    sprites_array[array_position].pos_x = x;
    sprites_array[array_position].pos_y = y;
    sprites_array[array_position].enable = sp;
    sprites_array[array_position].offset = offset;
    sprites_array[array_position].step_x = step_x;
    sprites_array[array_position].step_y = step_y;
    sprites_array[array_position].direction = direction;
    sprites_array[array_position].collision = 0;

    int ok = set_sprite(reg, x, y, offset, sp);
    return ok;
    
}

/**
 * \brief           Usada para colocar o valor 0 como o tamanho de todo os poligonos que estão na memoria, assim desativando ele
 */
void clear_poligonos()
{
    int i = 0;
    for (i; i < 15; i++)
    {
        set_poligono(i, 0, 0, 0, 0, 0, 0, 0);
    }
}

/**
 * \brief           Usada para desativar todos os sprite que estão nos registradores 1 até 31
 */
void clear_sprites()
{
    int i = 1;
    for (i; i < 32; i++)
    {
        set_sprite(i, 0, 0, 0, 0);
    }
}

void static_movement(Sprite *sp, uint8_t mirror)
{   
    if ((*sp).direction == 0) { /* DIREITA */
        //printf("PASSOU AQUI: mirroe %d\n", ((*sp).pos_x + (*sp).step_x) > 620);
        if (mirror == 1 && (*sp).pos_x + (*sp).step_x > 620) {
            //printf("PASSOU AQUI2\n");
            (*sp).pos_x = 0;
            set_sprite((*sp).data_register, 0, (*sp).pos_y, (*sp).offset, (*sp).enable);
        }
        else{
            int a = (*sp).pos_x + (*sp).step_x;
            (*sp).pos_x = a;
            set_sprite((*sp).data_register, a, (*sp).pos_y, (*sp).offset, (*sp).enable);
        }
    }
    else if ((*sp).direction == 1) { /*ESQUERDA */
        if (mirror == 1 && (*sp).pos_x - (*sp).step_x < 0) {
            (*sp).pos_x = 620;
            set_sprite((*sp).data_register, 620, (*sp).pos_y, (*sp).offset, (*sp).enable);
        }
        else {
            (*sp).pos_x = (*sp).pos_x - (*sp).step_x;
            set_sprite((*sp).data_register, (*sp).pos_x - (*sp).step_x, (*sp).pos_y, (*sp).offset, (*sp).enable);
        }
    }
    else if ((*sp).direction == 2) { /*BAIXO */
        
        if (mirror == 1 && (*sp).pos_y + (*sp).step_y > 460) {
            (*sp).pos_y = 0;
            set_sprite((*sp).data_register, (*sp).pos_x, 0, (*sp).offset, (*sp).enable);
        }
        else{
            int a = (*sp).pos_y + (*sp).step_y;
            (*sp).pos_y = a;
            set_sprite((*sp).data_register, (*sp).pos_x, a, (*sp).offset, (*sp).enable);
        }
    }
    else if ((*sp).direction == 3) { /*CIMA */
        if (mirror == 1 && (*sp).pos_y - (*sp).step_y < 0) {
            (*sp).pos_y = 460;
            set_sprite((*sp).data_register, (*sp).pos_x, 460, (*sp).offset, (*sp).enable);
        }
        else {
            (*sp).pos_y = (*sp).pos_y - (*sp).step_y;
            set_sprite((*sp).data_register, (*sp).pos_x, (*sp).pos_y - (*sp).step_y, (*sp).offset, (*sp).enable);
        }
    }
}
void clear_all() {
    clear_background_blocks();
    clear_poligonos();
    clear_sprites();
}

void reset_sprites() {
    int i = 0;
    for(i; i < 31; i++) {
        if (sprites_array[i].enable == 1) {
            set_sprite(sprites_array[i].data_register, sprites_array[i].pos_x, sprites_array[i].pos_y, sprites_array[i].offset, 1);
        }
    }
}