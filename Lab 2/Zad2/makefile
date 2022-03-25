CC = gcc
CFLAGS = -Wall -g


compile:
		$(CC) $(FLAGS) main.c -o main

run:  compile
		@echo " " >> pomiar_zad_2.txt
		@echo "for 'a' and 'Inwokacja.txt" >> pomiar_zad_2.txt
		./main a Inwokacja.txt

clean:
		rm -f main
