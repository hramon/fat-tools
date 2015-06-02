#ifndef FAT_DIRECTORY_H
#define FAT_DIRECTORY_H

#include "fat.h"

void FAT_make_directory_fat(fat_object* obj, char* path_directory);
unsigned char FAT_find_file_in_directory(fat_object* obj, char name[11], unsigned int* index, unsigned int* cluster_directory, fat_Directory_Entry* directory); /*returns 0 if not found, 1 if found*/

#endif