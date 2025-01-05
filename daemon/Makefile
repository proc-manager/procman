# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I./src
LDFLAGS = -lyaml

# Directories
SRC_DIR = src
IMPL_DIR = $(SRC_DIR)/impl
LIB_DIR = $(SRC_DIR)/lib

# Source files
SRC_MAIN = $(SRC_DIR)/main.c
IMPL_SRC = $(wildcard $(IMPL_DIR)/*.c)
OBJECTS = $(IMPL_SRC:.c=.o)

# Headers
HEADERS = $(wildcard $(LIB_DIR)/*.h)

# Output binaries
TARGET = procmand

# Rules
.PHONY: all clean

all: $(TARGET) 

$(TARGET): $(SRC_MAIN) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(IMPL_DIR)/*.o $(TARGET) 
