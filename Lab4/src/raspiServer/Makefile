# Compiler 
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# executables
EXEC = server

# Link to pthread library
LINK = -l pthread

# Source Files
SOURCE = server.c

# Object Files
OBJECTS = $(SOURCES:.c=.o)

all: $(EXEC)

$(EXEC): $(SOURCE)
	$(CC) -o $@ $^ $(LINK)

clean:
	rm -f $(OBJECTS) $(EXEC)

# Phony
.PHONY: clean all