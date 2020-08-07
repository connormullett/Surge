
CC=clang
CFLAGS=-g -pthread -Wall -Werror -fPIC
BINS=server scratch
LIBS=libtable.so
OBJS=table.o server.o thread_pool.o work_queue.o
DESTDIR=/usr/local

all: $(BINS) $(LIBS)

server: $(OBJS)
	$(CC) $(CFLAGS) -ltable -L. -o $@ $^

scratch: scratch.c
	$(CC) $(CFLAGS) -ltable -L. -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libtable.so: table.o
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm -r *.o
	rm -rf $(BINS)
