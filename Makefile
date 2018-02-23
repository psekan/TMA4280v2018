CC=gcc
CFLAGS="-O2"
LIBS=-lm
TARGET=pi
MAINSRC=main.c

SRC=$(wildcard *.c) $(wildcard riemann/*.c) $(wildcard machin/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

MAINOBJ=$(patsubst %.c, %.o, $(MAINSRC))

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

all: $(OBJ)
	$(CC) -o $(TARGET) -Wall $(LIBS) $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)
