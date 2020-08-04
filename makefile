
CC=clang
CFLAGS=-g -pthread
BINS=main scratch
OBJS=table.o main.o thread_pool.o work_queue.o

all: $(BINS)

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

scratch: scratch.c
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -r *.o
	rm -rf $(BINS)
