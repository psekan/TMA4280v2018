CC=mpicc
CFLAGS=-O2 -std=c99
LIBS=-lm
TARGET=pi
MAINSRC=main.c
UTESTSRC=unit-tests.c
UTESTTARGET=utest
VTESTSRC=verification-test.c
VTESTTARGET=vtest 

SRC=$(wildcard computation/*.c)
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

.DELETE_ON_ERROR:
vtest: $(OBJ)
	$(CC) -o $(VTESTTARGET) -Wall $(OBJ) $(VTESTSRC) $(LIBS)
	./$(VTESTTARGET)
	rm -f $(VTESTTARGET) 

clean:
	rm -f $(TARGET) $(OBJ)
