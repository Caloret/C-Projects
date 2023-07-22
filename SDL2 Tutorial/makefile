# REMEMBER including SDL2.dll | SDL2_image.dll 
# if compiling in Windows

SDL2_FOLDER=C:\\SDL2

CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11

ifeq ($(OS), Windows_NT)
    CINCLUDESDL2=-I "$(SDL2_FOLDER)\\include\\SDL2" -Dmain=SDL_main
    CLIBSDL2=-lmingw32 -L"$(SDL2_FOLDER)\\lib" -lSDL2main -lSDL2 -lSDL2_image -mwindows -mconsole    
    CWinFlags = -mwindows -mconsole
else
    CINCLUDESDL2=$(shell pkg-config --cflags sdl2)
    CLIBSDL2=$(shell pkg-config --libs sdl2) -lSDL2_image
    CWinFlags =
endif	

BINARY=Tutorial_09

OBJECTS=arcadia_sdl.o Tutorial_09.o

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(CLIBSDL2) 

arcadia_sdl.o: arcadia_sdl.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDESDL2)

Tutorial_09.o: Tutorial_09.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDESDL2)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDESDL2)

.PHONY :  run
run:
    ifeq ($(OS), Windows_NT)
    	$(BINARY).exe
    else
	    ./$(BINARY)
    endif

.PHONY :  clean
clean:
    ifeq ($(OS), Windows_NT)
	    del $(BINARY).exe		   
	    del $(OBJECTS)
    else
	    rm -rf $(BINARY) || true	   
	    rm -rf $(OBJECTS) || true
    endif

