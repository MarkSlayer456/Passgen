CC=gcc
OBJS=passgen.o
DEST=passgen

all: $(DEST)

$(DEST): $(OBJS)
	$(CC) -o $(DEST) $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o $(DEST)


