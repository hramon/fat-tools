#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	fat_object obj;
	internal_file* file;
	char* filename = (char*)malloc((strlen("/test.txt")+1)*sizeof(char));
	strcpy(filename,"/test.txt");
	//FAT_create_fat("test.img",FAT32,20*1024*1024);
	FAT_read_fat(&obj,"test.img");
	file = FAT_open_file(&obj,filename);
	FAT_close_file(&obj,file);

	free(filename);

	filename = (char*)malloc((strlen("/fat.h")+1)*sizeof(char));
	strcpy(filename,"/fat.h");

	FAT_copy_file_to_fat(&obj,"fat.h",filename);

	FAT_copy_file_from_fat(&obj,filename,"test.txt");
        
        /*test for splitting paths*/
        
        {
#define FN "/test/test2/"
            file_path* fp;
            free(filename);
            filename = (char*)malloc((strlen(FN)+1)*sizeof(char));
            strcpy(filename,FN);
            fp=FAT_split_path(filename);
            FAT_free_file_path(fp);
        }


	free(filename);
	filename = (char*)malloc((strlen("/folder")+1)*sizeof(char));
	strcpy(filename,"/folder");

	FAT_make_directory_fat(&obj,filename);

	free(filename);

	filename = (char*)malloc((strlen("/folder/main.c")+1)*sizeof(char));
	strcpy(filename,"/folder/main.c");

	FAT_copy_file_to_fat(&obj,"main.c",filename);

	FAT_close_fat(&obj);
	return 0;
}
