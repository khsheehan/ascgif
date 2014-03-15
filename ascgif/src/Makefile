IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) `pkg-config --cflags opencv`

ODIR=obj
LDIR =../lib

LIBS=-lm `pkg-config --libs opencv`

_DEPS = server.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o gifprocessor.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
