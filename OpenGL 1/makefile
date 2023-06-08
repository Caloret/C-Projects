CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11
OUTPUTFOLDER = .\bin
BINARY=$(OUTPUTFOLDER)\helloWindow

CFILES=glad.c helloWindow.c 
OBJECTS=$(OUTPUTFOLDER)\glad.o $(OUTPUTFOLDER)\helloWindow.o


all: $(BINARY)

$(BINARY): $(OBJECTS)
		   $(CC) -o $@ $^ -lglfw3 -lgdi32 -lopengl32

%.o:%.c
	$(CC) $(CFLAGS) -c -o $(OUTPUTFOLDER)\$@ $^

clean:
	del $(BINARY).exe		   
	del $(OUTPUTFOLDER)\*.o
