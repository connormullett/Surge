
CC=clang
CFLAGS=-g -pthread
BINS=main
OBJS=table.o main.o thread_pool.o work_queue.o

all: $(BINS)

main: main.o table.o thread_pool.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -r *.o
	rm -rf $(BINS)
