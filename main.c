#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	char* filename = (char*)malloc((strlen("/test.txt")+1)*sizeof(char));
	strcpy(filename,"/test.txt");
	fat_object obj;
	internal_file* file;
	create_fat("test.img",FAT32,20*1024*1024);
	read_fat(&obj,"test.img");
	file = open_file_fat(&obj,filename);
	close_file_fat(&obj,file);

	free(filename);

	filename = (char*)malloc((strlen("/fat.h")+1)*sizeof(char));
	strcpy(filename,"/fat.h");

	copy_file_to_fat(&obj,"fat.h",filename);

	copy_file_from_fat(&obj,filename,"test.text");

	close_fat(&obj);
	return 0;
}
