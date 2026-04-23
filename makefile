CC=gcc
CFLAGS=-lncurses -I. -ansi
DEPS=tictactoe.h blackjack.h
OBJ=arcade.o tictactoe.o blackjack.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

arcade: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

