CC = gcc
CFLAGS = -Wall -g

all:
	make run
	make clean

run:
	$(CC) $(FLAGS) main.c -o main

clean:
	rm -rf main

cleanTXT:
	rm *.txt

test:

	make run
	./main >> raport2.txt

	make clean
