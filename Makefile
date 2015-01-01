CC = gcc
LINK = ld
flags = -g
files = main.c fat.c

all: fattools
clean:
	rm fattools

fattools:$(files)
	$(CC) $(flags) $^ -o $@


