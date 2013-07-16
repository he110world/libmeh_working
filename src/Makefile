engine: backend.c backend.h game.c
	gcc -std=c99 -o engine backend.c -g -lGL -lGLU -msse2 `sdl-config --libs --cflags` `freetype-config --libs --cflags`\
	 -rdynamic -ldl

stb_image.o: stb_image.c
	gcc -O3 -c stb_image.c

game.so: stb_image.o game.c backend.h
	gcc -std=c99 -fPIE -c game.c -o game.o -O3 -fno-unit-at-a-time;
	gcc -shared -W1,-soname,game.so.1 -o game.so game.o stb_image.o;
