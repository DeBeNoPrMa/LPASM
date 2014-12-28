CFLAGS = -Wall -Wextra -std=c99

all: assembler errors structures main
	gcc-4.9 assembler.o errors.o structures.o main.o -o lpasm $(CFLAGS)


assembler: assembler.c assembler.h
	gcc-4.9 -c assembler.c $(CFLAGS)


errors: errors.h errors.c
	gcc-4.9 -c errors.c $(CFLAGS)


structures: structures.h structures.c
	gcc-4.9 -c structures.c $(CFLAGS)


main: main.c
	gcc-4.9 -c main.c $(CFLAGS)


clean:
	rm *.o
	rm lpasm
