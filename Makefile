CC=gcc
CFLAGS=-O2 -std=c99
LIBS=-lm
TARGET=pi
MAINSRC=main.c
UTESTSRC=test/unit-tests.c
UTESTTARGET=utest

SRC=$(wildcard riemann/*.c) $(wildcard machin/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

all: $(OBJ) $(MAINOBJ)
	$(CC) -o $(TARGET) -Wall $(OBJ) $(MAINSRC) $(LIBS)

.DELETE_ON_ERROR:
utest: $(OBJ) 
	$(CC) -o $(UTESTTARGET) -Wall $(OBJ) $(UTESTSRC) $(LIBS)
	./$(UTESTTARGET)
	rm -f $(UTESTTARGET) 

clean:
	rm -f $(TARGET) $(OBJ)
