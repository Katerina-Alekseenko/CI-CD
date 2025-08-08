SHELL := /bin/bash
TARGET = ./my_grep
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all : $(TARGET)

my_grep : my_grep.o
	$(CC) $(CFLAGS) my_grep.o -o $(TARGET)
	rm -rf *.o

my_grep.o : my_grep.c my_grep.h
	$(CC) $(CFLAGS) my_grep.c -c

test:
	bash tests.sh

format-check:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	rm .clang-format

format:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	rm .clang-format

clean :
	rm -r *.o my_grep

rebuild :
	clean my_grep
