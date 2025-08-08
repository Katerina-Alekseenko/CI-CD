CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TARGET = my_cat
SRC = my_cat.c
TEST_SCRIPT = t_cat.sh

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TARGET)
	@bash $(TEST_SCRIPT)

clean:
	rm -f $(TARGET) *.o log.txt

.PHONY: all test clean