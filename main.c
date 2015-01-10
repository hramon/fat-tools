#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	fat_object obj;
	internal_file* file;
	char* filename = (char*)malloc((strlen("/test.txt")+1)*sizeof(char));
	strcpy(filename,"/test.txt");
	create_fat("test.img",FAT32,20*1024*1024);
	read_fat(&obj,"test.img");
	file = open_file_fat(&obj,filename);
	close_file_fat(&obj,file);

	free(filename);

	filename = (char*)malloc((strlen("/fat.h")+1)*sizeof(char));
	strcpy(filename,"/fat.h");

	copy_file_to_fat(&obj,"fat.h",filename);

	copy_file_from_fat(&obj,filename,"test.txt");
        
        /*test for splitting paths*/
        
        {
#define FN "/test/test2/"
            file_path* fp;
            free(filename);
            filename = (char*)malloc((strlen(FN)+1)*sizeof(char));
            strcpy(filename,FN);
            fp=split_path(filename);
            free_file_path(fp);
        }

	close_fat(&obj);
	return 0;
}
