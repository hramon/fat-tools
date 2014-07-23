#include "fat.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
	fat_object obj;
	internal_file* file;
	create_fat("test.img",FAT32,20*1024*1024);
	read_fat(&obj,"test.img");
	file = open_file_fat(&obj,"/test.txt");
	close_file_fat(&obj,file);
	close_fat(&obj);
	return 0;
}