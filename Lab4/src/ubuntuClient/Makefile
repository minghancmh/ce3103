# Compiler 
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# executables
EXEC_CLIENT = client
EXEC_MULTICLIENT = multiclient

# Link to pthread library
LINK = -l pthread

# Source Files
SOURCES = client.c multiclient.c

# Object Files
OBJECTS = $(SOURCES:.c=.o)

all: $(EXEC_CLIENT) $(EXEC_MULTICLIENT)

$(EXEC_CLIENT): client.c
	$(CC) -o $@ $^ $(LINK)

$(EXEC_MULTICLIENT): multiclient.c
	$(CC) -o $@ $^ $(LINK)

clean:
	rm -f $(OBJECTS) $(EXEC_CLIENT) $(EXEC_MULTICLIENT)

# Phony
.PHONY: clean all