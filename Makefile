obj-m += gpu_driver.o

all: main gpu_driver.ko

gpu_driver.ko:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

main: main.c gpu_lib.c mouse_move.c draw_screens.c draw_sprites.c botoes.c display_controler.c
	gcc -o exec main.c gpu_lib.c mouse_move.c draw_screens.c draw_sprites.c botoes.c display_controler.c -lpthread

run: main
	sudo ./exec

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f exec