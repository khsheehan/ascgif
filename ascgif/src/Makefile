IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -I/usr/include/ImageMagick -I/usr/local/bin/ImageMagick-6.8.8/include/ImageMagick-6 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0 `pkg-config --cflags opencv`

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
