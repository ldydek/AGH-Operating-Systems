CC = gcc
CFLAGS = -Wall -g

compile:
	@$(CC) $(FLAGS) main.c -o main -lpthread

test: cleaner compile
	./main 1 numbers balloons.pgm negative.pgm
	./main 1 block balloons.pgm negative.pgm
	./main 2 numbers balloons.pgm negative.pgm
	./main 2 block balloons.pgm negative.pgm
	./main 4 numbers balloons.pgm negative.pgm
	./main 4 block balloons.pgm negative.pgm
	./main 8 numbers balloons.pgm negative.pgm
	./main 8 block balloons.pgm negative.pgm
	./main 16 numbers balloons.pgm negative.pgm
	./main 16 block balloons.pgm negative.pgm

cleaner:
	@rm -f main times.txt negative.pgm

clean:
	@rm -f main
