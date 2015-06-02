#ifndef FAT_PATH_H
#define FAT_PATH_H

#include "fat.h"

typedef struct _file_path{
    char** folderstructure; /*folders leading to the file or end folder*/
    unsigned int number_of_folders; /*amount of folders in the folderstructure array*/
}file_path;

file_path* FAT_split_path(char* path);
void FAT_free_file_path(file_path* path);
void FAT_filename_to_fat_name(char* filename, char* FAT_name);
unsigned char FAT_compare_fat_names(char name1[11],char name2[11]);


#endif