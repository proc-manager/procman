# Variables
CC := gcc
CFLAGS := -Wall -Wextra -I./src
LDFLAGS := 
SRC_DIR := src/impl
LIB_DIR := src/lib
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BUILD_DIR)/program

# Source and Object files
SRC := $(wildcard $(SRC_DIR)/*.c src/main.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(OBJ_DIR)/main.o
HEADERS := $(wildcard $(LIB_DIR)/*.h)

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< $(HEADERS) -o $@

$(OBJ_DIR)/main.o: src/main.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< $(HEADERS) -o $@

# Clean target
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
