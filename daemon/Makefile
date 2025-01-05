# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Iparser -Iisoproc

# Libraries
LIBS = -lyaml

# Source files
SRCS = main.c parser/parse_proc_spec.c isoproc/isoproc.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = procman

# Default target
all: $(EXEC)

# Build target
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean