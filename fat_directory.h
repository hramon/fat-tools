#ifndef FAT_DIRECTORY_H
#define FAT_DIRECTORY_H

#include "fat.h"

typedef struct _directory_item{
	unsigned char has_long_name:1;
	unsigned int long_name_entry_length;
	fat_Long_Name_Directory_entry* long_name;
	fat_Directory_Entry short_name;
	unsigned int start_entry;
}directory_item;


void FAT_make_directory_fat(fat_object* obj, char* path_directory);
unsigned char FAT_find_file_in_directory(fat_object* obj, char name[11], unsigned int* index, unsigned int* cluster_directory, fat_Directory_Entry* directory); /*returns 0 if not found, 1 if found*/
void FAT_read_directory_item(fat_object* obj,directory_item* item);
void FAT_write_directory_item(fat_object* obj,directory_item* item);


#endif