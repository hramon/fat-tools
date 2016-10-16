#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){


	//usage: fattools <fat_filename> <command> [<options>]

	if (argc <= 2) {
		return -1;
	}else if (strcmp(argv[2], "cpi") == 0) { //copy file from outside to the fat filesystem
		if (argc < 5)
			return -1;

		fat_object obj;
		FAT_read_fat(&obj, argv[1]);
		FAT_copy_file_to_fat(&obj, argv[3], argv[4]);
		FAT_close_fat(&obj);
	}else if (strcmp(argv[2], "cpo") == 0) { //copy file from the fat filesystem to the outside
		if (argc < 5)
			return -1;

		fat_object obj;
		FAT_read_fat(&obj, argv[1]);
		FAT_copy_file_from_fat(&obj, argv[3], argv[4]);
		FAT_close_fat(&obj);

	}else if (strcmp(argv[2], "cp") == 0) { //copy a file in the fat filesystem to another place in the fat filesystem
		if (argc < 5)
			return -1;

		fat_object obj;
		FAT_read_fat(&obj, argv[1]);
		FAT_copy_file(&obj, argv[3], argv[4]);
		FAT_close_fat(&obj);

	}else if (strcmp(argv[2], "rm") == 0) { //remove a file in the fat filesystem
		if (argc < 4)
			return -1;

		fat_object obj;
		FAT_read_fat(&obj, argv[1]);
		FAT_remove_file(&obj, argv[3]);
		FAT_close_fat(&obj);

	}else if (strcmp(argv[2], "create") == 0) { //create a fat filesystem
		if (argc < 4)
			return -1;

		FAT_create_fat(argv[1], FAT32, atoi(argv[3])*1024*1024);

	}else if (strcmp(argv[2], "mkdir") == 0) { //create a directory in fat filesystem
		if (argc < 4)
			return -1;
		fat_object obj;
		FAT_read_fat(&obj, argv[1]);
		FAT_make_directory_fat(&obj, argv[3]);
		FAT_close_fat(&obj);

	}else if (strcmp(argv[2], "rmdir") == 0) { //remove a directory in fat filesystem
		//first remove trailing / if needed and then treat the directory as a file
		fat_object obj;
		char* new_name = (char*)malloc((strlen(argv[3]) + 1)*sizeof(char));
		strcpy(new_name, argv[3]);
		if (new_name[strlen(new_name) - 1] == '/')
			new_name[strlen(new_name) - 1] = 0;
		FAT_read_fat(&obj, argv[1]);
		FAT_remove_file(&obj, new_name);
		FAT_close_fat(&obj);
		free(new_name);
	}
	return 0;
}
