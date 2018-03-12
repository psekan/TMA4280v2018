CC=mpicc
CFLAGS=-O2 -std=c99
LIBS=-lm
TARGET=pi
MAINSRC=main.c
NP?=5
RESULTDIR="../../Results"
RESULTTMPDIR="$(RESULTDIR)/raw-mach1"

SRC=$(wildcard computation/*.c) $(wildcard ../mach-base/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

all: $(OBJ) $(MAINOBJ)
	$(CC) -o $(TARGET) -Wall $(OBJ) $(MAINSRC) $(LIBS)

test: all
	mpirun -n $(NP) ./$(TARGET) 

raw-result: all
	mkdir -p $(RESULTTMPDIR)
	echo "n;error" > "$(RESULTTMPDIR)/error.csv"
	for number in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 ; do \
		echo $$number | mpirun -n $(NP) ./$(TARGET) \
			| grep "Error;" | sed "s/Error/$$number/" >> "$(RESULTTMPDIR)/error.csv" ; \
	done
	echo "np;wall time" > "$(RESULTTMPDIR)/wall-time.csv"
	for number in 2 3 5 9 17 33; do \
	  echo 16777216 | mpirun -n $$number ./$(TARGET) \
	    | grep "Wall time;" | sed "s/Wall time/$$number/" >> "$(RESULTTMPDIR)/wall-time.csv" ; \
	done

plot: raw-result
	Rscript plot.r $(RESULTTMPDIR)"/error.csv" "$(RESULTDIR)/mach1-error.png" "Error plot" "n" "error"
	Rscript plot.r $(RESULTTMPDIR)"/wall-time.csv" "$(RESULTDIR)/mach1-wall-time.png" "Wall time" "processes" "wall time"

clean:
	rm -f $(TARGET) $(OBJ)
