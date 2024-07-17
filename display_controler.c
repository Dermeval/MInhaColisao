#include "display_controler.h"

volatile int *HEX0_BASE_ptr, *HEX1_BASE_ptr, *HEX2_BASE_ptr, *HEX3_BASE_ptr, *HEX4_BASE_ptr, *HEX5_BASE_ptr;
int fd_display = -1;
void *LW_virtual;

int numbers(int c) {
    switch (c) {
        case 0: return 0b01000000;
        case 1: return 0b11111001;
        case 2: return 0b10100100;
        case 3: return 0b10110000;
        case 4: return 0b10011001;
        case 5: return 0b10010010;
        case 6: return 0b10000010;
        case 7: return 0b11111000;
        case 8: return 0b10000000;
        case 9: return 0b10010000;
    }
}

int open_display_device() {
     if ((fd_display = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERRO: não foi possível abrir \"/dev/mem\"...\n");
        return 0;
    }
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd_display, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERRO: mmap() falhou...\n");
        close(fd_display);
        return 0;
    }

    HEX0_BASE_ptr = (volatile int *)(LW_virtual + HEX0_BASE);
    HEX1_BASE_ptr = (volatile int *)(LW_virtual + HEX1_BASE);
    HEX2_BASE_ptr = (volatile int *)(LW_virtual + HEX2_BASE);
    HEX3_BASE_ptr = (volatile int *)(LW_virtual + HEX3_BASE);
    HEX4_BASE_ptr = (volatile int *)(LW_virtual + HEX4_BASE);
    HEX5_BASE_ptr = (volatile int *)(LW_virtual + HEX5_BASE);

    return 1;
}

void write_number_display(int display_digit, int number) {
    
    if (display_digit == 0) {
        *HEX0_BASE_ptr = numbers(number);
    } else if (display_digit == 1) {
        *HEX1_BASE_ptr = numbers(number);
    } else if (display_digit == 2) {
        *HEX2_BASE_ptr = numbers(number);
    } else if (display_digit == 3) {
        *HEX3_BASE_ptr = numbers(number);
    } else if (display_digit == 4) {
        *HEX4_BASE_ptr = numbers(number);
    } else if (display_digit == 5) {
        *HEX5_BASE_ptr = numbers(number);
    } 

}

void write_score (int score) {
    int dezena = score/10;
    int unidade = score % 10;

    write_number_display(1, dezena);
    write_number_display(0, unidade);

}

void write_lifes (int vidas) {
    int dezena = vidas/10;
    int unidade = vidas % 10;

    write_number_display(5, dezena);
    write_number_display(4, unidade);
}

void write_start_display() {
    write_number_display(0, 0);
    write_number_display(1, 0);
    write_number_display(2, 0);
    write_number_display(3, 0);
    write_number_display(4, 0);
    write_number_display(5, 0);
    
}

void close_display_device() {
    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd_display);
}
