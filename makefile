
CC=clang
CFLAGS=-g -pthread

main: main.o table.o thread_pool.o
	$(CC) $(CFLAGS) -o $@ $^
main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $^

table.o: table.c
	$(CC) $(CFLAGS) -c -o $@ $^

thread_pool.o: thread_pool.c
	$(CC) $(CFLAGS) -c -o $@ $^
