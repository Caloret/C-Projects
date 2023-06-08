CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11

CFILES=glad.c helloWindow.c 
OBJECTS=glad.o helloWindow.o

BINARY=bin

all: $(BINARY)

$(BINARY): $(OBJECTS)
		   $(CC) -o $@ $^ -lglfw3 -lgdi32 -lopengl32

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	del $(BINARY).exe		   
	del *.o
