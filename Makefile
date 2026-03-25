
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

TARGET = matrix_test

all: $(TARGET)

$(TARGET): matrix.c test_matrix.c
	$(CC) $(CFLAGS) matrix.c test_matrix.c -lcriterion -o $(TARGET)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
