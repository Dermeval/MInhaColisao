#include "botoes.h"

volatile int *KEY_ptr;
int fd_botoes = 0;
void *LW_virtual;

int botao0;
int botao1;
int botao2;
int botao3;

int open_botoes_device() {
    if ((fd_botoes = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERRO: não foi possível abrir \"/dev/mem\"...\n");
        return 0;
    }

    // Mapeia a memória
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd_botoes, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERRO: mmap() falhou...\n");
        close(fd_botoes);
        return 0;
    }

    KEY_ptr = (volatile int *)(LW_virtual + KEY_BASE);

    return 1;

}

void read_botoes() {
    botao0 = (*KEY_ptr & 0b1);
    botao1 = (*KEY_ptr & 0b10) >> 1;
    botao2 = (*KEY_ptr & 0b100) >> 2;
    botao3 = (*KEY_ptr & 0b1000) >> 3;
}

void close_botoes_devide() {
    // Desmapeia a memória e fecha o arquivo
    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd_botoes);
}