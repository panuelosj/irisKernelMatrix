net: net.o
	gcc -o net net.o -lpgplot -lcpgplot -lX11 -lm

net.o: net.c
	gcc -c net.c
