#ifndef FAT_PATH_H
#define FAT_PATH_H

#include "fat.h"

typedef struct _file_path{
    char** folderstructure; /*folders leading to the file or end folder*/
    unsigned int number_of_folders; /*amount of folders in the folderstructure array*/
}file_path;

file_path* split_path(char* path);
void free_file_path(file_path* path);
void filename_to_FAT_name(char* filename, char* FAT_name);
unsigned char compare_fat_names(char name1[11],char name2[11]);


#endif