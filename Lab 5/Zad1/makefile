.PHONY: main test
main:
	gcc -Wall -o main main.c

test:
	make main
	./main commands.txt "skladnik1 | skladnik2 | skladnik2"
clean:
	rm -f main
