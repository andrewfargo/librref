CC = gcc
CFLAGS = -O3

all: static dynamic

static: librref.a

dynamic: librref.so

librref.a: rref.o
	ar rcs $@ $^

librref.so: rref-fpic.o
	$(CC) -shared -o $@ $^


%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<

%-fpic.o: %.c %.h
	$(CC) -c $(CFLAGS) -fpic -o $@ $<

clean:
	rm -rf *.o

.PHONY: all static dynamic clean
