# REMEMBER including SDL2.dll | SDL2_image.dll 
# if compiling in Windows

SDL2_FOLDER=C:\\SDL2

CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11
CINCLUDESDL2=-I "$(SDL2_FOLDER)\\include\\SDL2" -Dmain=SDL_main
CLIBSDL2=-lmingw32 -L"$(SDL2_FOLDER)\\lib" -lSDL2main -lSDL2 -lSDL2_image -mwindows -mconsole

BINARY=Tutorial_07

OBJECTS=arcadia_sdl.o Tutorial_07.o

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(CLIBSDL2) 

arcadia_sdl.o: arcadia_sdl.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDESDL2)

Tutorial_07.o: Tutorial_07.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDESDL2)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDESDL2)

.PHONY :  run
run:
	$(BINARY).exe

.PHONY :  clean
clean:
	del $(BINARY).exe		   
	del $(OBJECTS)