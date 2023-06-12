CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11
OUTPUTFOLDER = .\bin
BINARY=$(OUTPUTFOLDER)\HelloWorld

CFILES=HelloWorld.c 
OBJECTS=$(OUTPUTFOLDER)\HelloWorld.o


all: $(BINARY)

$(BINARY): $(OBJECTS)
		   $(CC) -o $@ $^

%.o:%.c
	if not exist $(OUTPUTFOLDER) mkdir $(OUTPUTFOLDER)
	$(CC) $(CFLAGS) -c -o $(OUTPUTFOLDER)\$@ $^

clean:
	del $(BINARY).exe		   
	del $(OUTPUTFOLDER)\*.o
