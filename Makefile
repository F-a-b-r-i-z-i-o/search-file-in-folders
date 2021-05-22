CC = gcc
CFLAGS = -Wall -Werror -pedantic
OBJ = custom_find.o

.PHONY: clean

all: main

custom_find.o: custom_find.c
	@ echo "~~COMPILANDO"
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o custom_find $^ $(CFLAGS)
	@ echo "~~FINE COMPILAZIONE"

clean :
	-rm *.o
	-rm custom_find
