CC = gcc
CFLAGS = -Wall
ARG = date

test:
	make total_clean
	make main
	./main konrad@gmail.com konrad hej

main:
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f *.o *.a *.so

total_clean:
	make clean
	rm -f main
