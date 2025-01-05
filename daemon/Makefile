# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I. -Iisoproc -Iparser

# Directories
SRC_DIR = .
ISOPROC_DIR = isoproc
PARSER_DIR = parser

# Source files
ISOPROC_SRC = $(wildcard $(ISOPROC_DIR)/*.c)
PARSER_SRC = $(wildcard $(PARSER_DIR)/*.c)
MAIN_SRC = $(SRC_DIR)/main.c

SRC = $(MAIN_SRC) $(ISOPROC_SRC) $(PARSER_SRC)

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
TARGET = program

# Rules
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
    $(CC) $(OBJ) -o $@

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJ) $(TARGET)
