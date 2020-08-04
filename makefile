
CC=clang
CFLAGS=-g -pthread
BINS=server scratch
OBJS=table.o server.o thread_pool.o work_queue.o

all: $(BINS)

server: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

scratch: scratch.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -r *.o
	rm -rf $(BINS)
