#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>       
#include <sys/mman.h>
#include <unistd.h>  
#define KEY_BASE 0x0
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000


int main(void) {
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
            // Espera até o botão ser solto
            while ((*KEY_ptr & 0x1) == 0x1);
        }
        usleep(100000); // Espera por 100ms
    }

    // Desmapeia a memória e fecha o arquivo
    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}