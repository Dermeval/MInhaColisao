/**
 * \file            gpu_driver.c
 * \brief           Codigo para criação do kernel que vai se comunicar com a GPU
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

#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/delay.h>

/* Definição dos OPCODES das intruções */
#define WBR 0b00
#define WBM 0b10
#define WSM 0b01
#define DP  0b11

/* Endereço base de memorias */
#define DATA_A  0x80
#define DATA_B  0x70
#define START 0xc0
#define WRFULL 0xb0
#define LW_BRIDGE_BASE 0xFF200000 
#define LW_BRIDGE_SPAN 0x00005000 

#define DEVICE_NAME "gpu_driver"
#define CLASS_NAME "gpudriver_class"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matheus Mota, Pedro Henrique e Dermeval Neves");
MODULE_DESCRIPTION("Módulo de exemplo para envio de instruções");

// Declaração de variáveis globais
static int major_number;
static struct class* gpu_class = NULL;
static struct device* gpu_device = NULL;

volatile int *START_PTR;
volatile int *WRFULL_PTR;
volatile int *DATA_A_PTR;
volatile int *DATA_B_PTR;
void __iomem *LW_virtual;


static int device_open(struct inode *inodep, struct file *filep);
static int device_release(struct inode *inodep, struct file *filep);
static ssize_t device_write(struct file *filep, const char *buffer, size_t len, loff_t *offset);

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .write = device_write,
};

/**
 * \brief           Usada enviar a intrução para as duas filas (DATA_A e DATA_B) da GPU com base no endereço de memoria
 * 
 * \param[in]       opcode: Valor para a cor azul.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       R: Valor para a cor vermelha.
*/
void send_instruction(volatile int opcode_enderecamentos, volatile int dados) {

    iowrite32(0, START_PTR); /* Atualiza o sinal de start para 0 fazendo com as intruções não sejam enviada*/
    iowrite32(opcode_enderecamentos, DATA_A_PTR); /* Envia o OPCODE e o endereçamento necessario da intrução para a fila DATA_A*/
    iowrite32(dados, DATA_B_PTR); /* Envia os dados necessarios da intrução para a fila DATA_B*/
    iowrite32(1, START_PTR); /* Atualiza o sinal de start para 1 fazendo que as intruções sejam envidas */
    iowrite32(0, START_PTR); /* Atualiza o sinal de start para 0 fazendo com as intruções não sejam enviadas */
}

/**
 * \brief           Usada montas a intrução WBR que muda a cor do background.
 * 
 * \param[in]       B: Valor para a cor azul.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       R: Valor para a cor vermelha.
*/
void instrucao_wbr(int b, int g, int r) {
    volatile int opcode = WBR; /* Define o OPCODE da instrução */
    volatile int dados = (b << 6) | (g << 3) | r; /* Monta a intrução que sera enviada para a DATA_B com base no valor das cores do RGB */
    send_instruction(opcode, dados); /* Usa a função para enviar as intruções para as filas e executa elas */
}

void instrucao_wbr_sprite(int reg, int offset, int x, int y, int sp) {
    volatile int opcode = WBR; /* Define o OPCODE da instrução */
    volatile int opcode_reg = (reg << 4) | opcode ; /* Monta a instrução que era enviada para a DATA_A com bae no valor do registrador escolhido */
    volatile int dados = offset | (y << 9) | (x << 19); /* Monta a intrução que sera enviada para a DATA_B com base no valor de offset, x e y */
    if (sp) {
        dados |= (1 << 29); /* Atualiza a instrução montada com o valor de enable (sp) do sprite */
    }
    send_instruction(opcode_reg, dados); /* Usa a função para enviar as intruções para as filas e executa elas */
}

void instrucao_wbm(int address, int r, int g, int b) {
    volatile int opcode = WBM; /* Define o OPCODE da instrução */
    volatile int dados = (b << 6) | (g << 3) | r; /* Monta a intrução que sera enviada para a DATA_B com base na cor RGB do bloco */
    volatile int opcode_reg = (address << 4) | opcode; /* Monta a instrução que era enviada para a DATA_A com base no endereço recebido */
    send_instruction(opcode_reg, dados); /* Usa a função para enviar as intruções para as filas e executa elas */
}

void instrucao_wsm(int address, int r, int g, int b) {
    volatile int opcode = WSM; /* Define o OPCODE da instrução */
    volatile int dados = (b << 6) | (g << 3) | r; /* Monta a intrução que sera enviada para a DATA_B com base na cor RGB do pixel do sprite*/
    volatile int opcode_reg = (address << 4) | opcode; /* Monta a instrução que era enviada para a DATA_A com bae no endereço recebio */
    send_instruction(opcode_reg, dados); /* Usa a função para enviar as intruções para as filas e executa elas */
}

void instrucao_dp(int address, int ref_x, int ref_y, int size, int r, int g, int b, int shape) {
    volatile int opcode = DP; /* Define o OPCODE da instrução */
    volatile int opcode_reg = (address << 4) | opcode; /* Monta a instrução que era enviada para a DATA_A com base no endereço recebido */
    volatile int rgb = (b << 6) | (g << 3) | r; /* Monta uma parte da intrução onde tem os valores RGB */
    volatile int dados =  (rgb << 22) | (size << 18) | (ref_y << 9) | ref_x; /* Monta a intrução completa que sera enviada para a DATA_B */
    if (shape) {
        dados |= (1 << 31); /* Atualiza a intrução monstada com o valor do tipo do poligono*/
    }
    send_instruction(opcode_reg, dados); /* Usa a função para enviar as intruções para as filas e executa elas */
}

static int device_open(struct inode *inodep, struct file *filep) {
    return 0;
}

static int device_release(struct inode *inodep, struct file *filep) {
    return 0;
}

static ssize_t device_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)  {
    unsigned char command[9]; 
   
    /* Lê o valor de fila cheia */
    int buffer_gpu = ioread32(WRFULL_PTR);
    
    /* Loop para quando a fila esta cheia o driver dar uma pequena pausa para esperar a fila liberar*/
    while (buffer_gpu){
        buffer_gpu = ioread32(WRFULL_PTR);
        /*if (buffer_gpu == 0){
            msleep(130);
        }*/
    };
    
    /* Verifica se o commando recebido esta nos padrões aceitaveis pelo kernel */
    if (len < 4 || len > 7) {
        printk(KERN_ALERT "Comprimento de comando inválido\n");
        return -EINVAL;
    }

    if (copy_from_user(&command, buffer, sizeof(command))) {
        return -EFAULT;
    }

    /* Switch case que chama a função de montar instruções com bae no valor recebedido pelo kernel */
    switch (command[0]) {
        case 0:  { /* Intrução WBR de mudar cor do background */
            int r = command[1];
            int g = command[2];
            int b = command[3];
            instrucao_wbr(r, g, b);
            break;
        }
        case 1: { /* Intrução WBR para colocar sprites na tela */
            int reg = command[1];
            int offset = ((command[2] << 1) & 0x1FE) | ((command[3] >> 7) & 0x01); // 9-bit offset
            int x = ((command[3] << 3) & 0x3F8) | ((command[4] >> 5) & 0x07);     // 10-bit x
            int y = ((command[4] << 5) & 0x3E0) | ((command[5] >> 3) & 0x1F);     // 10-bit y
            int sp = command[6];
            instrucao_wbr_sprite(reg, offset, x, y, sp);
            break;
        }
        case 2: { /* Instrução WBM para desenhar background blocks na tela */
            int address = ((command[1] << 5) | (command[2] >> 3)); // 12-bit address
            int r = command[2] & 0b111;
            int g = command[3];
            int b = command[4];
            instrucao_wbm(address, r, g, b);
            break;
        }
        case 3: { /* Instrução WSM para mudar a cor de um pixel do sprite */
            int address = (command[1] << 6) | (command[2]); // 14-bit address
            int r = command[3];
            int g = command[4];
            int b = command[5];
            instrucao_wsm(address, r, g, b);
            break;
        }
        case 4: { /* Instrução DP para colocar um poligono na tela */
            int address = command[1];
            int ref_x = ((command[2] << 1) | command[3] >> 7);
            int ref_y = (((command[3] & 0b1111111) << 2) | command[4] >> 6);
            int size = command[4] & 0b1111;
            int r = command[5] >> 5;
            int g = (command[5] >> 2) & 0b111;
            int b =  command[6] >> 5;
            int shape =  command[6] & 0b1;
            instrucao_dp(address, ref_x, ref_y, size, r, g, b, shape);
            break;
        }
        default: { /* Caso o commando seja invalido o kernel envia um alerta */
            printk(KERN_ALERT "Comando desconhecido\n");
            return -EINVAL;
        }
    }

    return len;
}


static int __init my_module_init(void) {
    LW_virtual = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    major_number = register_chrdev(0, DEVICE_NAME, &fops);

    printk(KERN_INFO "por favor\n");

    if (major_number < 0) {
        printk(KERN_ALERT "Falha ao registrar um número principal\n");
        return major_number;
    }

    gpu_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(gpu_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Falha ao registrar a classe do dispositivo\n");
        return PTR_ERR(gpu_class);
    }

    printk(KERN_INFO "Módulo carregado: classe do dispositivo criada corretamente\n");
    gpu_device = device_create(gpu_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(gpu_device)) {
        class_destroy(gpu_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Falha ao criar o dispositivo\n");
        return PTR_ERR(gpu_device);
    }

    LW_virtual = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    if (!LW_virtual) {
        device_destroy(gpu_class, MKDEV(major_number, 0));
        class_unregister(gpu_class);
        class_destroy(gpu_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Falha ao mapear a memória\n");
        return -ENOMEM;
    }


    DATA_A_PTR = (volatile int *) (LW_virtual + DATA_A);
    DATA_B_PTR = (volatile int *) (LW_virtual + DATA_B);   
    START_PTR = (volatile int *) (LW_virtual + START);
    WRFULL_PTR = (volatile int *) (LW_virtual + WRFULL);
    
    return 0;
}


static void __exit my_module_exit(void) {
    iounmap(LW_virtual);
    device_destroy(gpu_class, MKDEV(major_number, 0));
    class_unregister(gpu_class);
    class_destroy(gpu_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Módulo descarregado\n");

}


module_init(my_module_init);
module_exit(my_module_exit);
