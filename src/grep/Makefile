CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TARGET = my_grep
TEST_SCRIPT = tests.sh

all: $(TARGET)

$(TARGET): my_grep.o
	$(CC) $(CFLAGS) $^ -o $@
	rm -f *.o

my_grep.o: my_grep.c my_grep.h
	$(CC) $(CFLAGS) -c $<

test: $(TARGET)
	@bash $(TEST_SCRIPT)

clean:
	rm -f *.o $(TARGET)

rebuild: clean all

.PHONY: all test clean rebuild