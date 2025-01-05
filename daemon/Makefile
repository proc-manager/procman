# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Directories
SRC_DIR = src
IMPL_DIR = $(SRC_DIR)/impl
LIB_DIR = $(SRC_DIR)/lib
EXAMPLES_DIR = examples/parser-event-helper

# Source files
SRC_MAIN = $(SRC_DIR)/main.c
IMPL_SRC = $(wildcard $(IMPL_DIR)/*.c)
EXAMPLE_MAIN = $(EXAMPLES_DIR)/main.c
OBJECTS = $(IMPL_SRC:.c=.o)

# Headers
HEADERS = $(wildcard $(LIB_DIR)/*.h)

# Output binaries
TARGET = daemon
EXAMPLE_TARGET = parser-event-helper

# Rules
.PHONY: all clean

all: $(TARGET) $(EXAMPLE_TARGET)

$(TARGET): $(SRC_MAIN) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(EXAMPLE_TARGET): $(EXAMPLE_MAIN) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(IMPL_DIR)/*.o $(TARGET) $(EXAMPLE_TARGET)
