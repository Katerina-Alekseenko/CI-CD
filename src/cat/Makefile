SHELL := /bin/bash
TARGET = ./my_cat
CC = gcc 
CFLAGS:=-Wall -Werror -Wextra -std=c11
RM = rm -rf 
SRC = my_cat.c

all: $(TARGET)

my_cat: my_cat.o
	$(CC) $(CFLAGS) my_cat.o -o $(TARGET)
	rm -rf *.o

my_cat.o: $(SRC) my_cat.h
	$(CC) $(CFLAGS) $(SRC) -c

test:
	bash t_cat.sh

format-check:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	rm .clang-format

format:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	rm .clang-format

clean:
	$(RM) *.o $(TARGET) log.txt my_cat.txt cat.txt

rebuild: clean $(TARGET)
