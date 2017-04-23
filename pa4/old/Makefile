CC = mpicc
CFLAGS = -std=gnu99 -g -Wall 
EXE = $(*.x)
LDLIBS = 
LDFLAGS = 


all: .mpicc

.mpicc: 
	mpicc -g -Wall -o main main.c ppm.c

clean:
	rm ./*.dSYM 
