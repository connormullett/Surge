
CC=clang
CFLAGS=-g -pthread -Wall -Werror -fPIC
BINS=server scratch
LIBS=libtable.so
OBJS=table.o server.o thread_pool.o work_queue.o cli.o
DESTDIR=/usr
LIBDIR=$(DESTDIR)/lib
INCDIR=$(DESTDIR)/include

all: $(LIBS) $(BINS)

server: $(OBJS)
	$(CC) $(CFLAGS) -ltable -o $@ $^

scratch: scratch.c cli.c
	$(CC) $(CFLAGS) -ltable -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libtable.so: table.o
	$(CC) $(CFLAGS) -shared -o $@ $^

install:
	$(shell mkdir -p $(INCDIR)/table)
	cp -r $(LIBS) $(LIBDIR)
	chmod 0755 $(LIBDIR)/$(LIBS)
	cp -r table.h $(INCDIR)/table
	ldconfig -n $(LIBDIR)

clean:
	rm -r *.o
	rm -r *.so
	rm -rf $(BINS)
