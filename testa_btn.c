#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "gpu_lib.h"
#include <stdlib.h>
#include <fcntl.h>       
#include <sys/mman.h>
 
#define KEY_BASE 0x0
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000

int main()
{   
    /* Tentar abrir o arquivo do kernel do driver da GPU */
    if (open_gpu_device() == 0)
        return 0;

    create_sprite(0, 15, 100, 100, 2, 1);
    create_sprite(1, 10, 100, 100, 0, 1);
    printf("cordX: %d, cordY: %d", sprites_array[0].pos_x, sprites_array[0].pos_y);
    int is_colliding = collision(&sprites_array[0], &sprites_array[1]);
    printf("\n\nCOLLISION: %d\n ", is_colliding);

    volatile int *KEY_ptr;
    int fd = -1;
    void *LW_virtual;

    // Abre /dev/mem
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERRO: não foi possível abrir \"/dev/mem\"...\n");
        return (-1);
    }

    // Mapeia a memória
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERRO: mmap() falhou...\n");
        close(fd);
        return (-1);
    }

    // Obtem o ponteiro para o endereço do botão
    KEY_ptr = (volatile int *)(LW_virtual + KEY_BASE);

    // Loop para testar o botão
    while (1) {
        if ((*KEY_ptr & 0) == 0) {
            printf("Botão pressionado!\n");
            set_background_color(0, 0, 7);
            // Espera até o botão ser solto
            
        }
        if ((*KEY_ptr & 0b10) == 0) {
            printf("A");
            set_background_color(7, 0, 0);
        }
        usleep(100000); // Espera por 100ms
    }

    // Desmapeia a memória e fecha o arquivo
    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;

    close_gpu_devide(); /* Fecha o arquivo do driver da GPU */

    return 0;
}