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
#include "botoes.h"
#include "display_controler.h"


    uint8_t game_state = 1;
    uint8_t start = 0;
    uint32_t contador = 0;

int coin_collumn = 0;
int coin_line = 0;
int is_game_state_on = 0;
int disable_sprite_player = 0;
int is_imortal = 0;
int action = 0;
int power_amount = 1;

int collisao_inimigos(Sprite *sp1, Sprite *sp2, int *invuneravel, int *vidas, int offset_imortal) {
    if (collision(sp1, sp2) && (*invuneravel) == 0) {
        if (action == 1) {
            action = 0;
            create_sprite(1, 1, pos_x, pos_y, 23, 0, 0, 0, 1);
            (*invuneravel) = 1;
            return -1;
        } else {
            if (*vidas > 0) {
                printf("VIDAS: %d\n", *vidas);
                *vidas -= 1;
                write_lifes(*vidas);
                create_sprite(1, 1, pos_x, pos_y, offset_imortal, 0, 0, 0, 1);
                *invuneravel = 1;
                return 0;
            } else {
                game_state = 4;
                contador = 0;
                is_game_state_on = 0;
                printf("COLISÃO: %d\n", sprites_array[1].collision);
                usleep(500000);
                return 1;
            }
        }
    }
    return -1;
}

int collisao_queijo(Sprite *sp1, Sprite *sp2, int *score, int *difficulty, int *difficulty_completed) {
    if (collision(sp1, sp2)) {
        *score += 1;
        printf("COIN: %d\n", *score);
        printf("PEGOU A MOEDA!\n");
        spawn_coin(difficulty);
        write_score(*score);
        if (*score == (((*difficulty) + 1) * 5)) {
            printf("AUSDHI");
            *difficulty_completed = 0;
            game_state = 2;
            usleep(500000);
            return 1;
        }
    }
    return -1;
}

int no_repeat_random (int last, int line) {
    int random_num = last;
    while (last == random_num) {
        if (line == 1) {
            random_num = rand() % 23;
        }
        else {
            random_num = rand() % 31;
        }
    }
    return random_num;
}

int no_repeat_random_and_coin (int last, int line) {
    int random_num = last;
    int ok = 1;
    int i = 3;
    while (ok) {
        i = 3;
        if (line == 1) {
            ok = 0;
            for (i; i < 7; i++) {
                if (random_num*20 == sprites_array[i].pos_y) {
                    ok = 1;
                }
            }
            random_num = rand() % 23;
        } else {
            ok = 0;
            for (i; i < 7; i++) {
                if (random_num*20 == sprites_array[i].pos_x) {
                    ok = 1;
                }
            }
        }
            random_num = rand() % 31;
    }
    return random_num;
}

int no_repeat_random_and_other (int last, int line, int difficulty) {
    int random_num = last;
    int ok = 1;
    int i = 3;
    if (difficulty == 3) {
        if (line == 1) {
            while (random_num*20 == sprites_array[2].pos_y || random_num*20 == sprites_array[3].pos_y) {
                random_num = rand() % 23;
            }
        } else {
            while (random_num*20 == sprites_array[2].pos_x || random_num*20 == sprites_array[3].pos_x) {
                random_num = rand() % 31;
            }
        }
    } else if (difficulty == 4) {
        if (line == 1) {
            while (random_num*20 == sprites_array[2].pos_y || random_num*20 == sprites_array[3].pos_y || random_num*20 == sprites_array[4].pos_y || 
                   random_num*20 == sprites_array[5].pos_y || random_num*20 == sprites_array[6].pos_y) {
                random_num = rand() % 23;
            }
        } else {
            while (random_num*20 == sprites_array[2].pos_x || random_num*20 == sprites_array[3].pos_x || random_num*20 == sprites_array[4].pos_x || 
                   random_num*20 == sprites_array[5].pos_x || random_num*20 == sprites_array[6].pos_x) {
                random_num = rand() % 31;
            }
        }
    }
    while (ok) {
        i = 2;
        if (line == 1) {
            ok = 0;
            for (i; i < 7; i++) {
                if (random_num*20 == sprites_array[i].pos_y) {
                    ok = 1;
                }
            }
            random_num = rand() % 23;
        } else {
            ok = 0;
            for (i; i < 7; i++) {
                if (random_num*20 == sprites_array[i].pos_x) {
                    ok = 1;
                }
            }
        }
            random_num = rand() % 31;
    }
    return random_num;
}

void spawn_bomb(int difficult) {
    int bomb_collumn = rand() % 31;
    int bomb_line = rand() % 23;
    bomb_collumn = no_repeat_random_and_other(bomb_collumn, 0, difficult);
    bomb_line = no_repeat_random_and_other(bomb_line, 1, difficult);
    create_sprite(3, 3, bomb_collumn*20, bomb_line*20, 2, 0, 0, 0, 1);
    
    bomb_collumn = no_repeat_random_and_other(bomb_collumn, 0, difficult);
    bomb_line = no_repeat_random_and_other(bomb_line, 1, difficult);
    create_sprite(4, 4, bomb_collumn*20, bomb_line*20, 2, 0, 0, 0, 1);

    if (difficult == 4) {
        bomb_collumn = no_repeat_random_and_other(bomb_collumn, 0, difficult);
        bomb_line = no_repeat_random_and_other(bomb_line, 1, difficult);
        create_sprite(5, 5, bomb_collumn*20, bomb_line*20, 2, 0, 0, 0, 1);
        
        bomb_collumn = no_repeat_random_and_other(bomb_collumn, 0, difficult);
        bomb_line = no_repeat_random_and_other(bomb_line, 1, difficult);
        create_sprite(6, 6, bomb_collumn*20, bomb_line*20, 2, 0, 0, 0, 1);
    }
}

void spawn_coin(int difficult) {
    srand(time(NULL));
    int last_line = coin_line;
    int last_collumn = coin_collumn;
    coin_collumn = no_repeat_random(coin_collumn, 0);
    coin_line = no_repeat_random(coin_line, 1);
    if (difficult == 3) {
        while ((coin_collumn*20 == sprites_array[3].pos_x || coin_collumn*20 == sprites_array[4].pos_x) || coin_collumn == last_collumn) {
            coin_collumn = no_repeat_random(coin_collumn, 0);
        }
    } else if (difficult == 4) {
        while ((coin_collumn*20 == sprites_array[3].pos_x || coin_collumn*20 == sprites_array[4].pos_x) ||
               (coin_collumn*20 == sprites_array[5].pos_x || coin_collumn*20 == sprites_array[6].pos_x)|| coin_collumn == last_collumn) {
            coin_collumn = no_repeat_random(coin_collumn, 0);
        }
    }
    create_sprite(2, 2, coin_collumn*20, coin_line*20, 5/*A DEFINIR OFFSET*/, 0, 0, 0, 1);
}

void spaw_enemy(int difficult) {

    srand(time(NULL));

    int random_column = 0;
    int random_line = 0;
    // 0 = DIREITA, 1 = ESQUERDA, 2 = BAIXO, 3 = CIMA

    if (difficult == 0) {
        random_column = rand() % 31;
        random_line = rand() % 23;
        create_sprite(31, 31, random_column*20, random_line*20, 1, 1, 5, 0, 1);
        random_column = no_repeat_random(random_column, 0);
        random_line = no_repeat_random(random_line, 1);
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
    } else if (difficult == 3) {
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
    } else if (difficult == 4) {
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
    int difficulty = 0;
    int difficulty_completed = 1;
    int score = 0;
    int spawn_coin_ver = 0;
    int vidas = 3;
    int offset_imortal = 4;
    read_botoes();
    printf("%d\n", botao0);
    set_background_color(0, 0, 0);
    draw_start_game_screen();

    int is_imortal_time = 75;

    while (botao0) { /* BOTAO MAIS A DIREITA INICIA */
        read_botoes();
        //draw_start_screen(); /* TEM QUE CRIAR*/
    }

    draw_fase_screen(1, 0);
    usleep(2000000);
    game_state = 1;
    int is_paused = 1;
    int contador_aux = 0;

    while (1) {
        read_botoes();
        if (botao0 == 0) { /* BOTAO MAIS A DIREITA RESETA */
            game_state = 3;
        } else if (botao2 == 0 && (game_state != 5 && game_state != 4)) { /* CONTINUA O JOGO EM CASO DE PAUSE*/
            game_state = 1;
        } else if (botao3 == 0) { /* BOTAO MAIS A ESQUERDA ENCERRA O JOGO*/
                    game_state = 6;
                    is_paused = 0;
                    clear_poligonos();
                    clear_sprites();
                }
        if (game_state == 0) { /* JOGO PAUSADO */
            if (is_paused == 0) {
                draw_pause_screen();
                is_paused = 1;
            }
        }
        else if (game_state == 1) { /* FASE ROLANDO */

            draw_bg_cenario();
            if (is_paused == 1) {
                create_sprite(1, 1, sprites_array[1].pos_x, sprites_array[1].pos_y, sprites_array[1].offset, 0, 0, 0, 1);
                create_sprite(2, 2, sprites_array[2].pos_x, sprites_array[2].pos_y, 5, 0, 0, 0, 1);
            }
            is_paused = 0;
            if (start == 0) {
                spaw_enemy(difficulty);
                if (difficulty >= 3) {
                    spawn_bomb(difficulty);
                }
                start = 1;
                spawn_coin(difficulty);
                contador = 0;
                pos_x = 0;
                pos_y = 0;
                create_sprite(1, 1, 0, 0, sprites_array[1].offset, 0, 0, 0, 1);
                write_lifes(vidas);
            }
            while (difficulty_completed) {
                read_botoes();
                if (botao0 == 0) {
                    game_state = 3;
                    clear_poligonos();
                    clear_sprites();
                    break;
                } else if (botao1 == 0) {
                    game_state = 0;
                    is_paused = 0;
                    clear_poligonos();
                    clear_sprites();
                    break;
                } else if (botao3 == 0) {
                    game_state = 6;
                    is_paused = 0;
                    clear_poligonos();
                    clear_sprites();
                    break;
                }
                if (difficulty == 0) {
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[31], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[30], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_queijo(&sprites_array[1], &sprites_array[2], &score, &difficulty, &difficulty_completed) == 1) {
                        break;
                    }
                    if (contador == 8000){
                        static_movement(&sprites_array[31], 1);
                        static_movement(&sprites_array[30], 1);
                        if (is_imortal == 1) {
                            contador_aux += 1;
                        }
                        contador = 0;
                    }
                    if (contador_aux == is_imortal_time) {
                        create_sprite(1, 1, pos_x, pos_y, 0, 0, 0, 0, 1);
                        contador_aux = 0;
                        is_imortal = 0;
                    }
                    contador += 1;
                } else if (difficulty == 1) {
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[31], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[30], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[29], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[28], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_queijo(&sprites_array[1], &sprites_array[2], &score, &difficulty, &difficulty_completed) == 1) {
                        break;
                    }
                    if (contador == 6500){
                        static_movement(&sprites_array[31], 1);
                        static_movement(&sprites_array[30], 1);
                        static_movement(&sprites_array[29], 1);
                        static_movement(&sprites_array[28], 1);
                        if (is_imortal == 1) {
                            contador_aux += 1;
                        }
                        contador = 0;
                    }
                    if (contador_aux == is_imortal_time) {
                        create_sprite(1, 1, pos_x, pos_y, 0, 0, 0, 0, 1);
                        contador_aux = 0;
                        is_imortal = 0;
                    }
                    contador += 1;
                } else if (difficulty == 2) {
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[31], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[30], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }  
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[29], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[28], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[27], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[26], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_queijo(&sprites_array[1], &sprites_array[2], &score, &difficulty, &difficulty_completed) == 1) {
                        break;
                    }
                    if (contador == 5000){
                        static_movement(&sprites_array[31], 1);
                        static_movement(&sprites_array[30], 1);
                        static_movement(&sprites_array[29], 1);
                        static_movement(&sprites_array[28], 1);
                        static_movement(&sprites_array[27], 1);
                        static_movement(&sprites_array[26], 1);
                        if (is_imortal == 1) {
                            contador_aux += 1;
                        }
                        contador = 0;
                    }
                    if (contador_aux == is_imortal_time) {
                        create_sprite(1, 1, pos_x, pos_y, 0, 0, 0, 0, 1);
                        contador_aux = 0;
                        is_imortal = 0;
                    }
                    contador += 1;
                } else if (difficulty == 3) {
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[31], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[30], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[29], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[28], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[27], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[26], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[4], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[3], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_queijo(&sprites_array[1], &sprites_array[2], &score, &difficulty, &difficulty_completed) == 1) {
                        break;
                    }
                    if (contador == 4500){
                        static_movement(&sprites_array[31], 1);
                        static_movement(&sprites_array[30], 1);
                        static_movement(&sprites_array[29], 1);
                        static_movement(&sprites_array[28], 1);
                        static_movement(&sprites_array[27], 1);
                        static_movement(&sprites_array[26], 1);
                        if (is_imortal == 1) {
                            contador_aux += 1;
                        }
                        contador = 0;
                    }
                    if (contador_aux == is_imortal_time) {
                        create_sprite(1, 1, pos_x, pos_y, 0, 0, 0, 0, 1);
                        contador_aux = 0;
                        is_imortal = 0;
                    }
                    contador += 1;
                } else if (difficulty == 4) {
                   if (collisao_inimigos(&sprites_array[1], &sprites_array[31], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[30], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[29], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[28], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[27], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[26], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[6], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[5], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[4], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_inimigos(&sprites_array[1], &sprites_array[3], &is_imortal, &vidas, offset_imortal) == 1) {
                        break;
                    }
                    if (collisao_queijo(&sprites_array[1], &sprites_array[2], &score, &difficulty, &difficulty_completed) == 1) {
                        game_state = 5;
                        is_game_state_on = 0;
                        break;
                    }
                    if (contador == 4000){
                        static_movement(&sprites_array[31], 1);
                        static_movement(&sprites_array[30], 1);
                        static_movement(&sprites_array[29], 1);
                        static_movement(&sprites_array[28], 1);
                        static_movement(&sprites_array[27], 1);
                        static_movement(&sprites_array[26], 1);
                        if (is_imortal == 1) {
                            contador_aux += 1;
                        }
                        contador = 0;
                    }
                    if (contador_aux == is_imortal_time) {
                        create_sprite(1, 1, pos_x, pos_y, 0, 0, 0, 0, 1);
                        contador_aux = 0;
                        is_imortal = 0;
                    }
                    contador += 1;
                }
            }
        } else if (game_state == 2) { /*FASE COMPLETA*/
            disable_sprite_player = 0;
            is_paused = 0;
            contador = 0;

            difficulty += 1;

            if(difficulty == 1){
                draw_fase_screen(2, 0);
            } else if(difficulty == 2){
                draw_fase_screen(3, 0);
            } else if(difficulty == 3){
                draw_fase_screen(4, 0);
            } else if(difficulty == 4){
                draw_fase_screen(5, 0);
            }
            usleep(2000000);
            game_state = 1;
            difficulty_completed = 1;
            start = 0;
        } else if (game_state == 3) { /* JOGO REINICIADO */
            is_paused = 0;
            disable_sprite_player = 0;
            vidas = 3;
            score = 0;
            power_amount = 1;
            int i = 0;
            for(i; i < 31; i++) {
                sprites_array[i].collision = 0;
                sprites_array[i].enable = 0;
                sprites_array[i].pos_x = 0;
                sprites_array[i].pos_y = 0;
            }
            write_score(score);
            clear_all();
            draw_restart_screen();
            usleep(2000000);
            difficulty = 0;
            difficulty_completed = 1;
            game_state = 1;
            start = 0;
        } else if (game_state == 4) { /* GAME OVER */
            if (is_game_state_on == 0) {
                disable_sprite_player = 0;
                printf("PASSOU AQUI\n");
                draw_game_over_screen();
                is_game_state_on = 1;
            }
        } else if (game_state == 5) { /* WIN */
            if (is_game_state_on == 0) {
                draw_win_screen();
                is_paused = 0;
                int i = 0;
                for(i; i < 31; i++) {
                    sprites_array[i].collision = 0;
                    sprites_array[i].enable = 0;
                    sprites_array[i].pos_x = 0;
                    sprites_array[i].pos_y = 0;
                }
                usleep(2000000);
                start = 0;
                is_game_state_on = 1;
            }
        } else if (game_state == 6) {
            draw_end_game_screen();
            break;
        }
    }
    return NULL;
}

void* player_movement(void* arg) {
    int contador_2 = 0;
    int contador_3 = 0;
    while (1) {
        if (game_state == 1) {
        mouse_movement(&action, &power_amount);
        if (action == 1) {
            create_sprite(1, 1, sprites_array[1].pos_x, sprites_array[1].pos_y, 6, 0, 0, 0, 1);
        }
        change_coordinate(&sprites_array[1], pos_x, pos_y, 0);
        } else if (game_state == 6) {
            break;
        } else if (disable_sprite_player == 0) {
            usleep(500000);
            create_sprite(1, 1, 0, 0, 0, 0, 0, 0, 0);
            disable_sprite_player = 1;
        }
        //printf("\nX = %d , Y = %d\n", pos_x, pos_y);
    }
    return NULL;
}

int main() {



    if (open_gpu_device() == 0)
        return 0;
    if (open_mouse_device() == 0){
        return 0;
    }
    if (open_botoes_device() == 0){
        return 0;
    }
    if (open_display_device() == 0){
        return 0;
    }

    
    draw_player();
    draw_enemy();
    draw_coin();
    draw_bomb();
    draw_bomb_animation();
    draw_ghost();
    draw_player_shild();

    clear_background_blocks();
    clear_poligonos();
    clear_sprites();

    set_background_color(0, 0, 0);

    write_start_display();

    create_sprite(1, 1, 0, 0, 0, 0, 0, 0, 0);  /* PLAYER */ 

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

    
    /*  
    for (int i = 0; i<32; i++) {
        if (sprites_array[i].enable == 1){
            printf("POSX=%d, POSY=%d\n", sprites_array[i].pos_x, sprites_array[i].pos_y);
        }
    } */




    close_gpu_devide();
    close_mouse_device();
    close_botoes_devide();
    close_display_device();

    return 0;
}