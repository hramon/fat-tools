#ifndef FAT_FILE_H
#define FAT_FILE_H

#include "fat.h"

typedef struct _internal_file{
	unsigned int start_cluster;
	unsigned int current_cursor;
	unsigned int current_cluster;
	unsigned int start_directory_entry;
	unsigned int current_total_cursor;
	fat_Directory_Entry file;
}internal_file;


#define FAT_eof(pfile) (pfile->file.DIR_FileSize<=pfile->current_total_cursor)

void FAT_copy_file_to_fat(fat_object* obj,char* file_to_copy,char* destination);
void FAT_copy_file_from_fat(fat_object* obj,char* file_to_copy,char* destination);
internal_file* open_file_fat(fat_object* obj,char* path);
void FAT_remove_file_fat(fat_object* obj,char* path);
void FAT_copy_file_fat(fat_object* obj,char* source,char* destination);
void FAT_clear_content_file_fat(fat_object* obj,internal_file* file);
void FAT_write_file_fat(fat_object* obj,internal_file* file,void * buffer, unsigned int size_buffer);
void FAT_read_file_fat(fat_object* obj,internal_file* file,void* buffer, unsigned int size_buffer);
void FAT_close_file_fat(fat_object* obj,internal_file* file);

#endif