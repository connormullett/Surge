
CC=clang
CFLAGS=-g -pthread -fPIC -Iheaders
BINS=build/server
LIBS=obj/libtable.so
OBJS=obj/table.o obj/server.o obj/thread_pool.o obj/work_queue.o obj/cli.o
DESTDIR=/usr
LIBDIR=$(DESTDIR)/lib
INCDIR=$(DESTDIR)/include

all: $(LIBS) $(BINS)

$(BINS): $(OBJS)
	$(CC) $(CFLAGS) -ltable -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

obj/libtable.so: obj/table.o
	$(CC) $(CFLAGS) -shared -o $@ $^

install:
	$(shell mkdir -p $(INCDIR)/table)
	cp -r $(LIBS) $(LIBDIR)
	chmod 0755 $(LIBDIR)/$(LIBS)
	cp -r table.h $(INCDIR)/table
	ldconfig -n $(LIBDIR)

clean:
	rm -r $(BINS) $(LIBS) $(OBJS)
