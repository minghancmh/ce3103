# Compiler
CC = clang

# Compiler Flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Debug Compiler Flags
DEBUG_CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -O0

# GPROF Flags
GPROF_CFLAGS = -Wall -Wextra -pedantic -std=c11 -pg

# Source Files
SOURCES = hello-main.c hello-funct1.c hello-funct2.c

# Object Files for Regular Build
OBJECTS = $(SOURCES:.c=.o)

# Object Files for Debug Build
DEBUG_OBJECTS = $(SOURCES:.c=_d.o)

# Object Files for GPROF Build
GPROF_OBJECTS = $(SOURCES:.c=_gp.o)

# Executable Names
EXEC = helloworld
DEBUG_EXEC = helloworld-d
GPROF_EXEC = helloworld-gp

# Default Target
all: $(EXEC) $(DEBUG_EXEC) $(GPROF_EXEC)

# Build the Regular Executable
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS)

# Build the Debug Executable
$(DEBUG_EXEC): $(DEBUG_OBJECTS)
	$(CC) $(DEBUG_CFLAGS) -o $(DEBUG_EXEC) $(DEBUG_OBJECTS)

# Build the GProf Executable
$(GPROF_EXEC) : $(GPROF_OBJECTS)
	$(CC) $(GPROF_CFLAGS) -o $(GPROF_EXEC) $(GPROF_OBJECTS)

# Compile .c Files to .o Files (Regular Build)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c Files to _d.o Files (Debug Build)
%_d.o: %.c
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

# Compile .c Files to _gp.o Files (GPROF Build)
%_gp.o: %.c
	$(CC) $(GPROF_CFLAGS) -c $< -o $@

# Clean Up Build Artifacts
clean:
	rm -f $(OBJECTS) $(DEBUG_OBJECTS) $(EXEC) $(DEBUG_EXEC) $(GPROF_OBJECTS) $(GPROF_EXEC)

# Phony Targets
.PHONY: clean all
