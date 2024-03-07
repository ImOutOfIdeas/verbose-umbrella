CC = gcc
CFLAGS = -g -Wall -std=c11

EXE = vum 
SRC_DIR = src

# Get all C files from the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

all: $(EXE)

$(EXE): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXE)
 
