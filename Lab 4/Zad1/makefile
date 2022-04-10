CC = gcc
CFLAGS = -Wall -g

all:
	make run_exec
	make run_fork
	make clean

run_exec:
	$(CC) $(FLAGS) main.c -o main -ldl -D EXEC

run_fork:
	$(CC) $(FLAGS) main.c -o main -ldl -D FORK


clean:
	rm -rf main

cleanTXT:
	rm *.txt

test:
	@echo " FOR FORK " >> raport2.txt
	@echo " " >> raport2.txt

	@echo "~~~~~~~IGNORE TEST~~~~~" >> raport2.txt
	make run_fork
	./main ignore >> raport2.txt
	@echo "" >> raport2.txt

	@echo "~~~~HANDLER TEST~~~~~~" >> raport2.txt
	make run_fork
	./main handler >> raport2.txt
	@echo "" >> raport2.txt

	@echo "~~~~~~MASK TEST~~~~~~ " >> raport2.txt
	make run_fork
	./main mask >> raport2.txt
	@echo "" >> raport2.txt


	@echo "~~~~~~PENDING TEST~~~~~ " >> raport2.txt
	make run_fork
	./main pending >> raport2.txt
	@echo "" >> raport2.txt
	@echo "" >> raport2.txt


	@echo " FOR EXEC " >> raport2.txt

	@echo "~~~~~~~IGNORE TEST~~~~~" >> raport2.txt
	make run_exec
	./main ignore >> raport2.txt
	@echo "" >> raport2.txt

	@echo "~~~~~~MASK TEST~~~~~~ " >> raport2.txt
	make run_exec
	./main mask >> raport2.txt
	@echo "" >> raport2.txt


	@echo "~~~~~~PENDING TEST~~~~~ " >> raport2.txt
	make run_exec
	./main pending >> raport2.txt
	@echo "" >> raport2.txt
	@echo "" >> raport2.txt


