
# REMEMBER including SDL2.dll | SDL2_image.dll 
# if compiling in Windows

RAYLIB_FOLDER=C:\\Raylib

CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11

ifeq ($(OS), Windows_NT)
    CINCLUDERAYLIB=-I "$(RAYLIB_FOLDER)\\include" 
    CLIBRAYLIB=-lmingw32 -L"$(RAYLIB_FOLDER)\\lib" -lraylib -lopengl32 -lgdi32 -lwinmm
    CWinFlags = 
else
    CINCLUDERAYLIB=$(shell pkg-config --cflags raylib)
    CLIBRAYLIB=$(shell pkg-config --libs raylib) 
    CWinFlags =
endif	

BINARY=Tutorial_02

OBJECTS=Tutorial_02.o

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBRAYLIB) 

Tutorial_02.o: Tutorial_02.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDERAYLIB)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CINCLUDERAYLIB)

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

