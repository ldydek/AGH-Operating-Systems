CC = gcc
CFLAGS = -Wall -g

all:
	make static

static:
	$(CC) $(CFLAGS) -c library.c -o library.o
	ar rcs library.a library.o


clean:
	@rm -rf *.o *.a *.so  main
	@rm -rf temp.txt

test_static: static
	$(CC) -c main.c
	$(CC) main.o library.a -o main

	rm -f raport2.txt

	echo "\t\t\t\tName\t\t\t\tReal [s]\tUser [s]\tSystem [s]\t" >> raport2.txt

	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files large1.txt stop_timer save_timer wc_files1L
	./main start_timer create_table 2 stop_timer save_timer create_table2 start_timer wc_files large1.txt stop_timer save_timer wc_files1L
	./main start_timer create_table 4 stop_timer save_timer create_table4 start_timer wc_files small1.txt small1.txt stop_timer save_timer wc_files2S
	./main start_timer wc_files small1.txt small1.txt small1.txt small1.txt stop_timer save_timer wc_files4S
	./main create_table 2 wc_files large1.txt large1.txt large1.txt read_data_from_file start_timer remove_block 0 stop_timer save_timer remove_blocks1
	./main create_table 2 wc_files large1.txt large1.txt read_data_from_file wc_files large1.txt medium1.txt read_data_from_file start_timer remove_block 0 remove_block 1 stop_timer save_timer remove_blocks2
	./main start_timer wc_files large1.txt large1.txt medium1.txt medium1.txt small1.txt stop_timer save_timer wc_files2L2M2S
	./main start_timer create_table 2 create_block 1 create_block 0 stop_timer save_timer create_block2

