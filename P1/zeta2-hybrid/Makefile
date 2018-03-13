CC=mpicc
CFLAGS=-Wall -fopenmp
LIBS=-lm
TARGET=pi
NP?=5

all: $(TARGET)

test: all
	mpirun -n $(NP) ./$(TARGET)

$(TARGET): main.c computation/zeta.c
	$(CC) $(CFLAGS) -o pi main.c computation/zeta.c $(LIBS)
