/**
 * \file            limpar.c
 * \brief           Arquivo que pode ser executado para limpar todos os elementos da tela
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
#include "gpu_lib.h"

int main()
{   
	/* Tentar abrir o arquivo do kernel do driver da GPU */
    if (open_gpu_device() == 0)
        return 0;

	set_background_color(0, 0, 0); /* Retorna o background para a cor preta */
    clear_background_blocks(); /* Limpa todos os background blocks */
	clear_poligonos(); /* Limpa todo os poligono */
	clear_sprites(); /* Limpa todos os sprites */

	close_gpu_devide(); /* Fecha o arquivo do driver da GPU */
}