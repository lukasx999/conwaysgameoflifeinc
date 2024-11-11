CC=gcc
CFLAGS=-Wall -Wextra
LIBS=-lraylib

all: main.o
	$(CC) $(CFLAGS) $(LIBS) $^ -o out

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@
