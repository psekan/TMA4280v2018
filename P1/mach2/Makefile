CC=gcc
CFLAGS=-O2 -std=c99
LIBS=-lm -fopenmp
TARGET=pi
MAINSRC=main.c

SRC=$(wildcard computation/*.c) $(wildcard ../mach-base/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

pi: $(OBJ) $(MAINOBJ)
	$(CC) -o $(TARGET) -Wall $(OBJ) $(MAINSRC) $(LIBS)

all: pi

test: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)
