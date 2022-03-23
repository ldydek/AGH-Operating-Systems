CC = gcc
CFLAGS = -Wall -g

all: run

compile:
		$(CC) $(FLAGS) main.c -o main

run:  compile
		./main file_from.txt file_to.txt

clean:
		rm -f *.o main *.exe
