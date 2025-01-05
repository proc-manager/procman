# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Directories
SRC_DIR = .
ISOPROC_DIR = $(SRC_DIR)/isoproc
PARSER_DIR = $(SRC_DIR)/parser

# Sources and objects
SRCS = $(SRC_DIR)/main.c \
       $(ISOPROC_DIR)/isoproc.c \
       $(PARSER_DIR)/parse_proc_spec.c

OBJS = $(SRCS:.c=.o)

# Output binary
TARGET = procmand

# Default target
all: $(TARGET)

# Build the target binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
