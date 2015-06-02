#include "fat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


unsigned char find_file_in_directory(fat_object* obj, char name[11], unsigned int* index, unsigned int* cluster_directory,fat_Directory_Entry* directory){

	unsigned int i;

    while(1){
        /*get current directory*/
		fseek(obj->file,cluster_cursor(obj,(*cluster_directory)),SEEK_SET);
        read_Directory_Entry(directory,obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/SIZE_DIRECTORY_ENTRY,obj->file);
        fflush(obj->file);

        for(i=0;i<obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/SIZE_DIRECTORY_ENTRY;i++){
            if(compare_fat_names(directory[i].DIR_Name,name)){
                /*we found the folder or file*/
				*index=i;
                return 1;
            }
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

void make_directory_fat(fat_object* obj,char* path_directory){

	unsigned int i;
	unsigned int index = 0;
    unsigned char found = 1;
	fat_Directory_Entry* directory = NULL;
	unsigned int current_directory_cluster = 0;
	unsigned int current_directory = obj->bpb.specific_per_fat_type.fat32.BPB_RootClus;
	file_path* path;

	path = split_path(path_directory);

	for(i=0;i<path->number_of_folders-1;i++){
		char name[11];
        
		filename_to_fat_name(path->folderstructure[i],name);

        directory = (fat_Directory_Entry*)malloc(sizeof(fat_Directory_Entry)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus);
		current_directory_cluster = current_directory;

		/*search for the name in the directory*/
		found=find_file_in_directory(obj,name,&index,&current_directory_cluster,directory);


		if(found){
			current_directory = (directory[index].DIR_FstClusHI<<16)+directory[index].DIR_FstClusLO;
		}else{
			/*we did not find the folder name and hence we abort*/
			free(directory);
			break;
		}

		free(directory);
	}


	if(found){

		char name[11];
		unsigned int dir;
		unsigned int i;
		fat_Directory_Entry entry;
		unsigned int first_free_cluster = 0;
		time_t t = time(NULL);
        struct tm* time = localtime(&t);
        
		filename_to_fat_name(path->folderstructure[path->number_of_folders-1],name);

		dir = find_next_free_dir_entry(obj,current_directory);

		entry.DIR_attr = ATTR_DIRECTORY;
		memcpy(&(entry.DIR_Name),name,11);
		entry.DIR_NTRes = 0;

		date_time(&(entry.DIR_CrtDate),&(entry.DIR_CrtTime));
        entry.DIR_WrtDate = entry.DIR_CrtDate;
        entry.DIR_WrtTime = entry.DIR_CrtTime;
        entry.DIR_LstAccDate = entry.DIR_CrtDate;
        entry.DIR_FileSize = 0;

		first_free_cluster = find_next_free_cluster(obj);
        entry.DIR_FstClusLO = first_free_cluster & 0x0000FFFF;
        entry.DIR_FstClusHI = first_free_cluster & 0xFFFF0000; 
		
		fseek(obj->file,dir,SEEK_SET);
		write_Directory_Entry(&entry,1,obj->file);


		/*make the . folder*/
		entry.DIR_Name[0]='.';
		for(i=1;i<11;i++){
			entry.DIR_Name[i]=' ';
		}

		fseek(obj->file,cluster_cursor(obj,first_free_cluster),SEEK_SET);
		write_Directory_Entry(&entry,1,obj->file);

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

		fseek(obj->file,cluster_cursor(obj,first_free_cluster)+SIZE_DIRECTORY_ENTRY,SEEK_SET);
		write_Directory_Entry(&entry,1,obj->file);
	}

}