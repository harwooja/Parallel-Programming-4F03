CC = mpicc
CFLAGS = -std=gnu99 -g -Wall 
EXE = $(*.x)

all: .mpicc

.mpicc: 
	mpicc -g -Wall -o main main.c

main: main.o
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm ./*.o 


