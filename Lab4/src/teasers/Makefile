# Compiler 
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# executables
EXEC_pt1 = pthread1
EXEC_pt2 = pthread2
EXEC_mut1 = mutex1

# Link to pthread library
LINK = -l pthread

# Source Files
SOURCES = pthread1.c pthread2.c

# Object Files
OBJECTS = $(SOURCES:.c=.o)

all: $(EXEC_pt1) $(EXEC_pt2) $(EXEC_mut1)

$(EXEC_pt1): pthread1.c
	$(CC) -o $@ $^ $(LINK)

$(EXEC_pt2): pthread2.c
	$(CC) -o $@ $^ $(LINK)

$(EXEC_mut1): mutex1.c
	$(CC) -o $@ $^ $(LINK)

clean:
	rm -f $(OBJECTS) $(EXEC_pt1) $(EXEC_pt2) $(EXEC_mut1)

# Phony
.PHONY: clean all