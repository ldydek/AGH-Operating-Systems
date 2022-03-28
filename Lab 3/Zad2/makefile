CC = gcc
CFLAGS = -Wall -g

all:
	make run


run:
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -rf main
	rm -rf a.out
	rm *.txt


test1:

	@echo "for rectangle width: 10^-9 and 10 processes: " >> raport2.txt
	make run
	./main 0.000000001 10 >> raport2.txt
	@echo "" >> raport2.txt


test2:

	@echo "for rectangle width: 10^-10 and 9 processes: " >> raport2.txt
	make run
	./main 0.0000000001 9 >> raport2.txt
	@echo "" >> raport2.txt

tests:
	make test1
	make test2
