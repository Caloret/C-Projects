CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11
BINARY=Examples

OBJECTS=arcadia_string_lib.o Examples.o

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ 

.PHONY :  run
run:
	$(BINARY).exe

.PHONY :  clean
clean:
	del $(BINARY).exe		   
	del $(OBJECTS)