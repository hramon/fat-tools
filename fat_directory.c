#include "fat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


unsigned char FAT_find_file_in_directory(fat_object* obj, char* name, unsigned int* index, unsigned int* cluster_directory,directory_item* item){

	unsigned int i;

    while(1){

        /*get current directory*/
		fseek(obj->file,FAT_cluster_cursor(obj,(*cluster_directory)),SEEK_SET);

        for(i=0;i<obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/SIZE_DIRECTORY_ENTRY;){

			FAT_read_directory_item(obj, item);

            if(strcmp(FAT_get_directory_item_name(item),name)==0){
                /*we found the folder or file*/
				*index=i;
                return 1;
            }

			i++;

			if (item->has_long_name)
				i += item->long_name_entry_length;
        }

        /*find the next cluster in the directory*/
        fseek(obj->file,obj->start_fat+(*cluster_directory)*4,SEEK_SET);
        fread(cluster_directory,sizeof(unsigned int),1,obj->file);
        fflush(obj->file);
        if(*cluster_directory >= 0x0FFFFFF8){
                return 0;
        }
    }
}

void FAT_make_directory_fat(fat_object* obj,char* path_directory){

	unsigned int i;
	unsigned int index = 0;
    unsigned char found = 1;
	unsigned int current_directory_cluster = 0;
	unsigned int current_directory = obj->bpb.specific_per_fat_type.fat32.BPB_RootClus;
	file_path* path;

	path = FAT_split_path(path_directory);

	for(i=0;i<path->number_of_folders-1;i++){

		directory_item* item = (directory_item*)malloc(sizeof(directory_item));
		current_directory_cluster = current_directory;

		/*search for the name in the directory*/
		found=FAT_find_file_in_directory(obj, path->folderstructure[i],&index,&current_directory_cluster,item);


		if(found){
			current_directory = (item->short_name.DIR_FstClusHI<<16)+ item->short_name.DIR_FstClusLO;
		}else{
			/*we did not find the folder name and hence we abort*/
			free(item);
			break;
		}

		free(item);
	}


	if(found){

		char name[11];
		unsigned int dir;
		unsigned int i;
		directory_item item_entry;
		fat_Directory_Entry entry;
		unsigned int first_free_cluster = 0;
        
		FAT_filename_to_fat_name(path->folderstructure[path->number_of_folders-1],name);

		FAT_create_directory_item(&item_entry, path->folderstructure[path->number_of_folders - 1]);

		dir = FAT_find_next_free_dir_entry(obj,current_directory,item_entry.long_name_entry_length + 1);

		item_entry.short_name.DIR_attr = ATTR_DIRECTORY;
		memcpy(&(item_entry.short_name.DIR_Name),name,11);
		item_entry.short_name.DIR_NTRes = 0;

		FAT_date_time(&(item_entry.short_name.DIR_CrtDate),&(item_entry.short_name.DIR_CrtTime));
		item_entry.short_name.DIR_WrtDate = item_entry.short_name.DIR_CrtDate;
		item_entry.short_name.DIR_WrtTime = item_entry.short_name.DIR_CrtTime;
		item_entry.short_name.DIR_LstAccDate = item_entry.short_name.DIR_CrtDate;
		item_entry.short_name.DIR_FileSize = 0;

		first_free_cluster = FAT_find_next_free_cluster(obj);
		item_entry.short_name.DIR_FstClusLO = first_free_cluster & 0x0000FFFF;
		item_entry.short_name.DIR_FstClusHI = first_free_cluster & 0xFFFF0000;
		
		fseek(obj->file,dir,SEEK_SET);
		FAT_write_directory_item(obj,&item_entry);


		/*make the . folder*/
		entry.DIR_Name[0]='.';
		for(i=1;i<11;i++){
			entry.DIR_Name[i]=' ';
		}

		fseek(obj->file,FAT_cluster_cursor(obj,first_free_cluster),SEEK_SET);
		FAT_write_Directory_Entry(&entry,1,obj->file);

		/*make the .. folder*/
		entry.DIR_Name[0]='.';
		entry.DIR_Name[1]='.';
		for(i=2;i<11;i++){
			entry.DIR_Name[i]=' ';
		}

		if(path->number_of_folders <=2){
			entry.DIR_FstClusLO = current_directory & 0x0000FFFF;
			entry.DIR_FstClusHI = current_directory & 0xFFFF0000;
		}else{
			entry.DIR_FstClusLO = 0;
			entry.DIR_FstClusHI = 0;
		}

		fseek(obj->file,FAT_cluster_cursor(obj,first_free_cluster)+SIZE_DIRECTORY_ENTRY,SEEK_SET);
		FAT_write_Directory_Entry(&entry,1,obj->file);
	}

}

void FAT_read_directory_item(fat_object* obj,directory_item* item){

	//first read as a directory entry, if it is a long name entry, convert and read the rest
	//otherwise return

	FAT_read_Directory_Entry(&item->short_name,1,obj->file);

	if((item->short_name.DIR_attr&ATTR_LONG_NAME)!=ATTR_LONG_NAME){
		item->has_long_name = 0;
		item->long_name_entry_length = 0;
		item->long_name = NULL;
	}else{
		fat_Long_Name_Directory_entry temp;
		int i;
		//rewind and read

		fseek(obj->file,-SIZE_DIRECTORY_ENTRY,SEEK_CUR);
		FAT_read_Long_Name_Directory_Entry(&temp,obj->file);

		item->long_name_entry_length = temp.LDIR_Ord&REVERSE_LAST_LONG_ENTRY;
		item->long_name = (fat_Long_Name_Directory_entry*)malloc(item->long_name_entry_length*sizeof(fat_Long_Name_Directory_entry));
		memcpy(item->long_name+item->long_name_entry_length-1,&temp,sizeof(fat_Long_Name_Directory_entry));

		/*The long name is contiguous*/
		for(i = 1; i < item->long_name_entry_length; i++){
			FAT_read_Long_Name_Directory_Entry(&(item->long_name[item->long_name_entry_length-i-1]),obj->file);
		}

		FAT_read_Directory_Entry(&item->short_name,1,obj->file);

		item->has_long_name = 1;
	}
}

void FAT_write_directory_item(fat_object* obj,directory_item* item){

	if (item->has_long_name) {
		int i;
		for (i = 0; i < item->long_name_entry_length; i++) {
			FAT_write_Long_Name_Directory_Entry(item->long_name + item->long_name_entry_length + i - 1, obj->file);
		}
	}
	
	FAT_write_Directory_Entry(&(item->short_name), 1, obj->file);
}

char* FAT_get_directory_item_name(directory_item * item){
	
	int i;
	char* fat_name;
	wchar_t* temp_name;


	if (item->has_long_name) {
		fat_name = (char*)calloc(item->long_name_entry_length * 13 * sizeof(wchar_t) + 1, sizeof(char));
		fat_name[item->long_name_entry_length * 13 * sizeof(wchar_t)] = '\0'; /*make sure it is null terminated*/

		temp_name = (wchar_t*)calloc(item->long_name_entry_length * 13 + 1, sizeof(wchar_t));
		temp_name[item->long_name_entry_length * 13] = 0; /*make sure it is null terminated*/

		for (i = 0; i < item->long_name_entry_length; i++) {
			memcpy(temp_name + i * 13, &(item->long_name[i].LDIR_Name1), 5 * sizeof(wchar_t));
			memcpy(temp_name + i * 13 + 5, &(item->long_name[i].LDIR_Name2), 6 * sizeof(wchar_t));
			memcpy(temp_name + i * 13 + 11, &(item->long_name[i].LDIR_Name3), 2 * sizeof(wchar_t));
		}

		wcstombs(fat_name, temp_name, item->long_name_entry_length * 13 * sizeof(wchar_t) + 1);

	} else {
		fat_name = (char*)calloc(12, sizeof(char));
		memcpy(fat_name, &(item->short_name.DIR_Name), 11);
	}
	
	return fat_name;
}

void FAT_create_directory_item(directory_item* item, char* name) {
	int i;
	wchar_t* temp_name;
	int length = mbstowcs(NULL,name,strlen(name));
	item->long_name_entry_length = length / 13 + 1;
	item->has_long_name = 1;

	item->long_name = (fat_Long_Name_Directory_entry*)calloc(item->long_name_entry_length, sizeof(fat_Long_Name_Directory_entry));
	temp_name = (wchar_t*)calloc(length+1, sizeof(wchar_t));
	mbstowcs(temp_name, name, length + 1);
	length++;
	for (i = 0; i < item->long_name_entry_length; i++) {

		memcpy(&(item->long_name[i].LDIR_Name1), temp_name + i * 13, (length>= 5 ? 5:length) * sizeof(wchar_t));
		length = length<=5?0:length-5;
		memcpy(&(item->long_name[i].LDIR_Name2), temp_name + i * 13 + 5, (length>= 6 ? 6:length) * sizeof(wchar_t));
		length = length<=6?0:length-6;
		memcpy(&(item->long_name[i].LDIR_Name3), temp_name + i * 13 + 11, (length>= 2 ? 2:length) * sizeof(wchar_t));
		length = length<=2?0:length-2;

		item->long_name[i].LDIR_Attr = ATTR_LONG_NAME;
		item->long_name[i].LDIR_Ord = i+1;
	}

	item->long_name[item->long_name_entry_length - 1].LDIR_Ord |= LAST_LONG_ENTRY;

	free(temp_name);
}
