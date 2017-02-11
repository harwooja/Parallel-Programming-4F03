CC = gcc
CFLAGS = -std=gnu99 -Wall -fopenmp -g
LDFLAGS = -fopenmp
LDLIB = -fopenmp
EXE = pa2.x
#SRC = sstr.c fileio.c pa2.c
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=*.o)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIB) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ) $(EXE) write.txt

run: $(EXE)
	./$(EXE) 0 3 6 3 a b c
