SHELL := /bin/bash

# ==================================================
# COMMANDS

# CC = arm-linux-gnueabihf-gcc
CC = gcc
RM = rm -f
MKDIR = mkdir -p

# ==================================================
# DIRECTORIES

SRC = src
BIN = bin
CFLAGS = -lpthread
HEADERS = headers
DIRS = src bin  headers

# ==================================================
# TARGETS

EXEC = app

# ==================================================
# COMPILATION

all: $(DIRS) $(EXEC)

$(DIRS):
	$(MKDIR) $@

app: $(SRC)/app.c $(SRC)/client.c $(SRC)/server.c
	$(CC) $^ -o $(BIN)/$@ -I $(HEADERS) $(CFLAGS)

clean:
	$(RM) $(OBJ)/*.o log/*

purge: clean
	$(RM) $(addprefix $(BIN)/, $(EXEC))
