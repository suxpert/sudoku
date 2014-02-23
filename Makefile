# Makefile for GNUmake
#

CC = gcc
LD = gcc
CFLAGS = -Wall -std=c99
RM = rm

OBJ = main.o\
      sudoku.o

SRC = $(OBJ:.o=.c)

TARGET = sudoku.exe

$(TARGET): $(OBJ)
	$(LD) $(CFLAGS) $^ -o $@

main.o: main.c sudoku.h
	$(CC) $(CFLAGS) -c $< -o $@

sudoku.o: sudoku.c sudoku.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONE: clean exec
clean:
	$(RM) $(TARGET) $(OBJ)

exec: $(TARGET)
	./$(TARGET)

