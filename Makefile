CC=gcc
CFLAGS="-O2"
LIBS=-lm
TARGET=bin/P1
MAINSRC=main.c

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, obj/%.o, $(SRC))

MAINOBJ=$(patsubst %.c, obj/%.o, $(MAINSRC))

obj/%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

all: $(MAINOBJ)
	$(CC) -o $(TARGET) -Wall $(LIBS) $(MAINOBJ)
	echo Compiling successful.

clean:
	rm -f $(TARGET) $(OBJ)