CC = gcc
CFLAGS = -Wall -g

all:
		make static
		make shared

static:
		$(CC) $(CFLAGS) -c library.c -o library.o
		ar rcs library.a library.o

shared:
		$(CC) $(CGLAGS) -fPIC -c library.c -o library.o
		$(CC) $(CFLAGS) -shared -o liblibrary.so library.o

dynamic:
		make shared
		$(CC) $(CFLAGS) main.c -o main.o -ldl -D DYNAMIC

clean:
		@rm -rf *.o *.a *.so  main
		@rm -rf *.txt

static_test:
	$(CC) -c main.c
	$(CC) main.o library.a -o main


	echo "\t\t\t\t\t\tStatic test" >> raport3a.txt

	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files ../Zad2/large1.txt stop_timer save_timer wc_files1L >> raport3a.txt
	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files ../Zad2/large1.txt stop_timer save_timer wc_files1L >> raport3a.txt
	./main start_timer create_table 2 create_block 1 create_block 0 stop_timer save_timer create_block2 >> raport3a.txt

shared_test:
	$(CC) -c main.c
	$(CC) main.o liblibrary.so -o main

	echo "\t\t\t\t\t\tShared test" >> raport3a.txt
	echo "\t\t\t\tName\t\t\t\tReal [s]\tUser [s]\tSystem [s]\t" >> raport3a.txt

	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files ../Zad2/large1.txt stop_timer save_timer wc_files1L >> raport3a.txt
	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files ../Zad2/large1.txt stop_timer save_timer wc_files1L >> raport3a.txt
	./main start_timer create_table 2 create_block 1 create_block 0 stop_timer save_timer create_block2 >> raport3a.txt

dynamic_test:
	$(CC) -c main.c
	$(CC) main.o liblibrary.so -o main

	echo "\t\t\t\t\t\tDynamic test" >> raport3a.txt
	echo "\t\t\t\tName\t\t\t\tReal [s]\tUser [s]\tSystem [s]\t" >> raport3a.txt


	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files ../Zad2/large1.txt stop_timer save_timer wc_files1L >> raport3a.txt
	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files ../Zad2/large1.txt stop_timer save_timer wc_files1L >> raport3a.txt
	./main start_timer create_table 2 create_block 1 create_block 0 stop_timer save_timer create_block2 >> raport3a.txt
