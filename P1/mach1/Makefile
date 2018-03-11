CC=mpicc
CFLAGS=-O2 -std=c99
LIBS=-lm
TARGET=pi
MAINSRC=main.c
NP?=5

SRC=$(wildcard computation/*.c) $(wildcard ../mach-base/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

all: $(OBJ) $(MAINOBJ)
	$(CC) -o $(TARGET) -Wall $(OBJ) $(MAINSRC) $(LIBS)

test: all
	mpirun -n $(NP) ./$(TARGET) 

clean:
	rm -f $(TARGET) $(OBJ)
