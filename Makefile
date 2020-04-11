OUT=./obj
CC = gcc
LIBS = -lSDL2
CFLAGS = -Wall

chip8: main.o screen.o keys.o core.o timer.o
	$(CC) $(CFLAGS) -o $@ obj/*.o $(LIBS)

%.o: src/%.c
	$(CC) $(CFLAGS) -DDEBUG -c -o $(OUT)/$@ $<

prepare:
	mkdir -p $(OUT)/obj

clean:
	rm -rf $(OUT) chip8

all: clean prepare chip8
