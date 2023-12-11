CFLAGS = -g -Wall -Werror -std=c99
CC = gcc
TARGET = example
SRC_DIR = src
# Source files (include all .c files in the current directory)
SRCS = $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)


all: $(TARGET)

.PHONY: clean full run all memoryCheck

# Link the executable from object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lc

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the program with parameters
run: $(TARGET)
	./$(TARGET)

full:
	make clean
	make
	make run

# To execute valgrind checks
memoryCheck:
	make clean
	make
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --log-file=log.txt --track-origins=yes -s ./$(TARGET)
