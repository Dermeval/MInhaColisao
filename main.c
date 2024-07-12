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
#include <pthread.h>


    uint8_t game_state = 1;
    uint8_t start = 0;
    uint32_t contador = 0;


int no_repeat_random (int last, int line) {
    int random_num = last;
    while (last == random_num) {
        if (line == 1) {
            random_num = rand() % 24;
        }
        else {
            random_num = rand() % 32;
        }
    }
    return random_num;
}

void spaw_enemy (int difficult) {
    int i = 0;
    int j = 0;

    srand(time(NULL));

    int random_column = 0;
    int random_line = 0;
    // 0 = DIREITA, 1 = ESQUERDA, 2 = BAIXO, 3 = CIMA

    if (difficult == 0) {
        random_column = rand() % 31;
        random_line = rand() % 23;
        printf("X=%d, Y=%d\n", random_column, random_line);
        create_sprite(31, 31, random_column*20, random_line*20, 1, 1, 5, 0, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1);
        printf("X=%d, Y=%d\n", random_column, random_line);
        create_sprite(30, 30, random_column*20, random_line*20, 1, 5, 1, 2, 1);
    }
    else if (difficult == 1) {
        random_column = rand() % 31;
        random_line = rand() % 23;
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(31, 31, random_column*20, random_line*20, 1, 2, 2, 0, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1);
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(30, 30, random_column*20, random_line*20, 1, 2, 2, 2, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1);
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(29, 29, random_column*20, random_line*20, 1, 2, 2, 1, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1   );
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(28, 28, random_column*20, random_line*20, 1, 2, 2, 3, 1);
    }

    else if (difficult == 2) {
        random_column = rand() % 31;
        random_line = rand() % 23;
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(31, 31, random_column*20, random_line*20, 1, 2, 2, 0, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1);
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(30, 30, random_column*20, random_line*20, 1, 2, 2, 2, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1);
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(29, 29, random_column*20, random_line*20, 1, 2, 2, 1, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1   );
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(28, 28, random_column*20, random_line*20, 1, 2, 2, 3, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1);
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(27, 27, random_column*20, random_line*20, 1, 2, 2, 0, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1   );
        //printf("X=%d, Y=%d\n", random_column*19, random_line*19);
        create_sprite(26, 26, random_column*20, random_line*20, 1, 2, 2, 2  , 1);
    }

}

void* loop_principal (void* arg) {

    while (1) {
            
        if (game_state == 0) {
        }
        else if (game_state == 1) {
            if (start == 0) {
                spaw_enemy(2);
                start = 1;
            }
            if (collision(&sprites_array[1], &sprites_array[31])) {
            game_state = 0;
            usleep(500000);
            printf("COLISÃO: %d\n", sprites_array[1].collision);
            clear_poligonos();
            clear_sprites();
            draw_game_over_screen();
            break;
            //printf("\nCOLIDIU COM A BOMBA NO CANTO SUPERIOR ESQUERDO!\n");
            }
            if (collision(&sprites_array[1], &sprites_array[30])) {
            game_state = 0;
            usleep(500000);
            printf("COLISÃO: %d\n", sprites_array[1].collision);
            clear_poligonos();
            clear_sprites();
            draw_game_over_screen();
            break;
            //printf("\nCOLIDIU COM A BOMBA NO CANTO SUPERIOR ESQUERDO!\n");
            }
            if (collision(&sprites_array[1], &sprites_array[29])) {
            game_state = 0;
            usleep(500000);
            printf("COLISÃO: %d\n", sprites_array[1].collision);
            clear_poligonos();
            clear_sprites();
            draw_game_over_screen();
            break;
            //printf("\nCOLIDIU COM A BOMBA NO CANTO SUPERIOR ESQUERDO!\n");
            }
            if (collision(&sprites_array[1], &sprites_array[28])) {
            game_state = 0;
            usleep(500000);
            printf("COLISÃO: %d\n", sprites_array[1].collision);
            clear_poligonos();
            clear_sprites();
            draw_game_over_screen();
            break;
            //printf("\nCOLIDIU COM A BOMBA NO CANTO SUPERIOR ESQUERDO!\n");
            }
            if (collision(&sprites_array[1], &sprites_array[27])) {
            game_state = 0;
            usleep(500000);
            printf("COLISÃO: %d\n", sprites_array[1].collision);
            clear_poligonos();
            clear_sprites();
            draw_game_over_screen();
            break;
            //printf("\nCOLIDIU COM A BOMBA NO CANTO SUPERIOR ESQUERDO!\n");
            }
            if (collision(&sprites_array[1], &sprites_array[26])) {
            game_state = 0;
            usleep(500000);
            printf("COLISÃO: %d\n", sprites_array[1].collision);
            clear_poligonos();
            clear_sprites();
            draw_game_over_screen();
            break;
            //printf("\nCOLIDIU COM A BOMBA NO CANTO SUPERIOR ESQUERDO!\n");
            }
            if (contador == 20000){
                static_movement(&sprites_array[31], 1);
                static_movement(&sprites_array[30], 1);
                static_movement(&sprites_array[29], 1);
                static_movement(&sprites_array[28], 1);
                static_movement(&sprites_array[27], 1);
                static_movement(&sprites_array[26], 1);
                contador = 0;
            }
            contador += 1;
        }
    }
    return NULL;
}

void* player_movement(void* arg) {
    while (game_state == 1) {
        //printf("\nX = %d , Y = %d\n", pos_x, pos_y);
        mouse_movement();
        change_coordinate(&sprites_array[1], pos_x, pos_y, 0);
        printf("\n");
    }
    usleep(500000);
    set_sprite(1, 0, 0, 0, 0);
    return NULL;
}

int main() {



    if (open_gpu_device() == 0)
        return 0;
    if (open_mouse_device() == 0){
        return 0;
    }

    draw_bomb();
    draw_coin();
    draw_player();
    draw_enemy();

    clear_background_blocks();
    clear_poligonos();
    clear_sprites();

    create_sprite(1, 1, 0, 0, 0, 0, 0, 0, 1);  /* PLAYER */ 

    pthread_t thread1, thread2;

    // Create the first thread
    if (pthread_create(&thread1, NULL, loop_principal, NULL) != 0) {
        perror("Failed to create thread1");
        return 1;
    }

    // Create the second thread
    if (pthread_create(&thread2, NULL, player_movement, NULL) != 0) {
        perror("Failed to create thread2");
        return 1;
    }

            // Wait for the threads to finish (they won't in this case, as they run indefinitely)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    set_background_color(0, 0, 0);
    
    /*  
    for (int i = 0; i<32; i++) {
        if (sprites_array[i].enable == 1){
            printf("POSX=%d, POSY=%d\n", sprites_array[i].pos_x, sprites_array[i].pos_y);
        }
    } */




    close_gpu_devide();
    close_mouse_device();

    return 0;
}