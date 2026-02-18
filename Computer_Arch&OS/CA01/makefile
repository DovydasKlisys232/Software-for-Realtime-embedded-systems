# This is a makefile for creating the executable, running it or removing executable and objects

# c code compiler
CC = gcc
# flags for compilaton. -Wall adds warnings
CFLAGS = -Wall

# Directories
LIB_BIN = bin
LIB_OBJ = build
LIB_SRC = src

# Source and object files 
SRC=$(wildcard $(LIB_SRC)/*.c)
OBJECTS=$(patsubst $(LIB_SRC)/%.c,$(LIB_OBJ)/%.o, $(SRC))

# The name of executable files
EXE = $(LIB_BIN)/ca1.exe

# default target
all: $(EXE)
	
# removes all executables from bin folder and all objects from buid folder
clean:
	@echo "Removing all executables and objects"
	rm -f $(LIB_BIN)/*.exe $(LIB_OBJ)/*.o 


$(EXE): $(OBJECTS)
	@echo "Creating executable of $(SRC) in the bin directory"
	@mkdir -p $(LIB_BIN)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# pattern to compile each .c into .o
$(LIB_OBJ)/%.o: $(LIB_SRC)/%.c
	@mkdir -p $(LIB_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ 

run:
	./$(EXE) 
