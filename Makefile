CC = gcc
LINK = ld
flags = -g
files = main.c fat_object.c fat_path.c fat_file.c fat_directory.c

all: fattools
clean:
	rm fattools

fattools:$(files)
	$(CC) $(flags) $^ -o $@


