CC = gcc
LIBS = -lSDL
CFLAGS = -Wall

chip8: main.o screen.o stack.o keys.o
	$(CC) $(CFLAGS) -o $@ obj/*.o $(LIBS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o obj/$@ $<

prepare:
	mkdir obj

clean:
	rm -rf ./obj chip8

all: clean prepare chip8
