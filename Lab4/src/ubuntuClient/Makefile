# Compiler 
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# executables
EXEC = client

# Link to pthread library
LINK = -l pthread

# Source Files
SOURCES = client.c

# Object Files
OBJECTS = $(SOURCES:.c=.o)

all: $(EXEC)

$(EXEC): client.c
	$(CC) -o $@ $^ $(LINK)

clean:
	rm -f $(OBJECTS) $(EXEC)

# Phony
.PHONY: clean all