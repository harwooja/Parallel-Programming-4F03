EXECS=pa4
MPICC?=mpicc

all: ${EXECS}

pa4: pa4.c ppmFile.c
	${MPICC} pa4.c ppmFile.c -o pa4.x 

clean:
	rm -f ${EXECS}

