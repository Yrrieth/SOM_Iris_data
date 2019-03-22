CC=gcc
SRC=iris.c main.c
OBJ=$(SRC:.c=.o)
FLAGS=-Wall

iris:	$(OBJ) iris.h
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lm

%.o:	%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm iris $(OBJ) *~

mrproper:
	rm iris