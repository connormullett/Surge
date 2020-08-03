
CC=clang
CFLAGS=-g

main: main.o table.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $^

table.o: table.c
	$(CC) $(CFLAGS) -c -o $@ $^
