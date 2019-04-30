SHELL := /bin/bash


# ==================================================
# COMMANDS

CC = gcc
RM = rm -f
MKDIR = mkdir -p

# ==================================================
# DIRECTORIES

SRC = src
BIN = bin
OBJ = obj
DIRS = src bin obj log

# ==================================================
# TARGETS

EXEC = sampling_a sampling_b

# ==================================================
# COMPILATION

all: $(DIRS) sampling_a sampling_b

$(DIRS):
	$(MKDIR) $@

# -- add any dependencies here

sampling_a: $(OBJ)/sampling_a.o
	$(CC) $^ -o $(BIN)/$@

sampling_b: $(OBJ)/sampling_b.o
	$(CC) $^ -o $(BIN)/$@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $^

clean:
	$(RM) $(OBJ)/*.o log/*

purge: clean
	$(RM) $(addprefix $(BIN)/, $(EXEC))
