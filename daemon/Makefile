# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Iparser

# Libraries
LIBS = -lyaml

# Source files
SRCS = main.c parser/parse_proc_spec.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = procman

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean