#include "fat.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void remove_file_fat(fat_object* obj,char* path){
	
	internal_file* file = open_file_fat(obj,path);
	clear_content_file_fat(obj,file);

	file->file.DIR_Name[0] = DIR_FREE;

	close_file_fat(obj,file);
}

void copy_file_fat(fat_object* obj,char* source,char* destination){

	internal_file* source_file;
	internal_file* destination_file;

	source_file = open_file_fat(obj,source);
	destination_file = open_file_fat(obj,destination);

	clear_content_file_fat(obj,destination_file);
	//memcpy(&(destination_file->file),&(source_file->file));

	close_file_fat(obj,destination_file);
	close_file_fat(obj,source_file);
}

void clear_content_file_fat(fat_object* obj,internal_file* file){
    
	unsigned int temp_cluster;
    unsigned int buffer = 0;

    if(file->file.DIR_FileSize == 0)
        return;
    
    file->current_cluster = file -> start_cluster;
    file->current_cursor = 0;

    if(file->start_cluster < obj->fs_info.FSI_Nxt_Free)
        obj->fs_info.FSI_Nxt_Free = file->start_cluster;

    fseek(obj->file,obj->start_fat+(file->current_cluster)*sizeof(unsigned int),SEEK_SET);
    fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
    fseek(obj->file,-sizeof(unsigned int),SEEK_CUR);
    fwrite(&buffer,sizeof(unsigned int),1,obj->file);

    while(temp_cluster != 0 && temp_cluster < 0x0FFFFFF8){
            if(temp_cluster < obj->fs_info.FSI_Nxt_Free)
                    obj->fs_info.FSI_Nxt_Free = temp_cluster;
            fseek(obj->file,obj->start_fat+(temp_cluster)*sizeof(unsigned int),SEEK_SET);
            fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
            fseek(obj->file,-sizeof(unsigned int),SEEK_CUR);
            fwrite(&buffer,sizeof(unsigned int),1,obj->file);
    }

    file->file.DIR_FileSize = 0;
    file->start_cluster = file->current_cluster = 0;
}

void write_file_fat(fat_object* obj,internal_file* file,void * buffer, unsigned int size_buffer){
    if(file->file.DIR_FileSize == 0 && size_buffer != 0){
        /*we need to allocate a new cluster for the file*/
        int first_free_cluster = find_next_free_cluster(obj);
        file->current_cluster = first_free_cluster;
        file->start_cluster = first_free_cluster;
        file->file.DIR_FstClusLO = first_free_cluster & 0x0000FFFF;
        file->file.DIR_FstClusHI = first_free_cluster & 0xFFFF0000; 
    }else if(size_buffer == 0){
        return;
    }
    
    fseek(obj->file,cluster_cursor(obj,file->current_cluster)+file->current_cursor,SEEK_SET);
    if(size_buffer + file->current_cursor <= (unsigned int)obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus){
            fwrite(buffer,size_buffer,1,obj->file);
            fflush(obj->file);
            file->current_cursor+=size_buffer;
            file->file.DIR_FileSize+=size_buffer;
    }else{
            unsigned int before = (obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus - file->current_cursor);
            unsigned int rest = size_buffer - before;
            unsigned int temp_cluster;

            fwrite(buffer,before,1,obj->file);
            fflush(obj->file);
            fseek(obj->file,obj->start_fat+(file->current_cluster)*sizeof(unsigned int),SEEK_SET);
            fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
            fflush(obj->file);
            file->current_cursor = 0;
            if(temp_cluster >= 0x0FFFFFF8){
                    /*allocate new cluster*/
                    unsigned int next_cluster = find_next_free_cluster(obj);
                    fseek(obj->file,obj->start_fat+(file->current_cluster)*sizeof(unsigned int),SEEK_SET);
                    fwrite(&next_cluster,sizeof(unsigned int),1,obj->file);
                    fflush(obj->file);
                    temp_cluster = next_cluster;
                    obj->fs_info.FSI_Free_Count--;
            }
            file->current_cluster = temp_cluster;
            file->file.DIR_FileSize+=before;
            file->current_cursor = 0;
            write_file_fat(obj,file,((unsigned char*)buffer)+before,rest);
    }

	date_time(&(file->file.DIR_WrtDate),&(file->file.DIR_WrtTime));
	date_time(&(file->file.DIR_LstAccDate),&(file->file.DIR_WrtTime));
}

void read_file_fat(fat_object* obj,internal_file* file,void* buffer, unsigned int size_buffer){
	unsigned short dummy;
	if(file->current_total_cursor + size_buffer <= file->file.DIR_FileSize){
		fseek(obj->file,cluster_cursor(obj,file->current_cluster)+file->current_cursor,SEEK_SET);
		if(size_buffer + file->current_cursor <= (unsigned int)obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus){
			fread(buffer,size_buffer,1,obj->file);
			fflush(obj->file);
			file->current_cursor+=size_buffer;
			file->current_total_cursor+=size_buffer;
		}else{
			unsigned int before = (obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus - file->current_cursor);
			unsigned int rest = size_buffer - before;
			unsigned int temp_cluster;
		
			fread(buffer,before,1,obj->file);
			fseek(obj->file,obj->start_fat+(file->current_cluster)*sizeof(unsigned int),SEEK_SET);
			fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
			file->current_cursor = 0;
			file->current_cluster = temp_cluster;
			file->current_total_cursor+=before;
			read_file_fat(obj,file,((unsigned char*)buffer)+before,rest);
		}
	}else if(!eof(file)){
		read_file_fat(obj,file,buffer,file->file.DIR_FileSize-file->current_total_cursor);
	}

	date_time(&(file->file.DIR_LstAccDate),&(dummy));
}

internal_file* open_file_fat(fat_object* obj,char* path){
    /*search for the file*/
    /*if the folder does not exist, quit, if the file does not exist, make it*/

    int i;	
    file_path* fp = NULL;
    unsigned int current_directory = obj->bpb.specific_per_fat_type.fat32.BPB_RootClus;

    /*make the file object*/
    internal_file* file = (internal_file*)calloc(1,sizeof(internal_file));


    fp = split_path(path);

    for(i=0;i<fp->number_of_folders;i++){
        char name[11];
        unsigned int index = 0;
        unsigned char found = 0;
		fat_Directory_Entry* directory = NULL;
		unsigned int current_directory_cluster = 0;

        enum{
                FIND_FOLDER,
                FIND_FILE
        }find_type;


        if(i==fp->number_of_folders-1)
            find_type = FIND_FILE;
        else
            find_type = FIND_FOLDER;

        filename_to_fat_name(fp->folderstructure[i],name);

        directory = (fat_Directory_Entry*)malloc(sizeof(fat_Directory_Entry)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus);
        current_directory_cluster = current_directory;

		/*search for the name in the directory*/
		found=find_file_in_directory(obj,name,&index,&current_directory_cluster,directory);

		if(found){
			if(find_type == FIND_FILE){
                /*we can stop here*/
                memcpy(&(file->file),&directory[index],SIZE_DIRECTORY_ENTRY);
                file->current_cluster = (directory[index].DIR_FstClusHI<<16)+directory[index].DIR_FstClusLO;
                file->current_cursor = 0;
                file->start_cluster = file->current_cluster;
                file->start_directory_entry = index*SIZE_DIRECTORY_ENTRY+cluster_cursor(obj,current_directory_cluster);
                free(directory);
				free_file_path(fp);
                return file;
            }else{
                current_directory = (directory[index].DIR_FstClusHI<<16)+directory[index].DIR_FstClusLO;
            }
		}else{
			if(find_type == FIND_FILE){
                /*allocate new here*/
                time_t t = time(NULL);
                struct tm* time = localtime(&t);
                unsigned int new_file = find_next_free_dir_entry(obj,current_directory);

                file->current_cluster = 0;
                file->start_cluster = 0;
                file->current_cursor = 0;
                file->start_directory_entry =new_file;

                memcpy(file->file.DIR_Name,name,11);
				date_time(&(file->file.DIR_CrtDate),&(file->file.DIR_CrtTime));
                file->file.DIR_WrtDate = file->file.DIR_CrtDate;
                file->file.DIR_WrtTime = file->file.DIR_CrtTime;
                file->file.DIR_LstAccDate = file->file.DIR_CrtDate;
                file->file.DIR_FileSize = 0;
                file->file.DIR_FstClusHI = file->file.DIR_FstClusLO = 0;

                free(directory);
				free_file_path(fp);
                return file;
            }else{
                /*we could create a new folder here*/
                free(file);
                free(directory);
				free_file_path(fp);
                return NULL;
            }
		}

        free(directory);

    }

	free_file_path(fp);
    
    return NULL;
}

void close_file_fat(fat_object* obj,internal_file* file){
	fseek(obj->file,file->start_directory_entry,SEEK_SET);
	write_Directory_Entry(&(file->file),1,obj->file);
	fflush(obj->file);
	free(file);
}

void copy_file_from_fat(fat_object* obj,char* file_to_copy,char* destination){
	/*open the file to copy*/
	internal_file* source;
	FILE* file = fopen((const char*)destination,"wb");

	/*open source handle*/
	source = open_file_fat(obj, file_to_copy);


	/*write the file to the disk*/
	while(!eof(source)){
		unsigned char buffer;
		read_file_fat(obj,source,&buffer,sizeof(buffer));
		fwrite(&buffer,sizeof(buffer),1,file);
	}


	/*close the file*/
	close_file_fat(obj,source);
}

void copy_file_to_fat(fat_object* obj,char* file_to_copy,char* destination){
	/*open the file to copy*/
	internal_file* dest;
	unsigned char buffer;
	FILE* file = fopen((const char*)file_to_copy,"rb");	

	/*open destination handle*/
	dest = open_file_fat(obj, destination);

	clear_content_file_fat(obj,dest);

	/*write the file to the disk*/
	while(fread(&buffer,sizeof(buffer),1,file)){
		write_file_fat(obj,dest,&buffer,sizeof(buffer));
	}


	/*close the file*/
	close_file_fat(obj,dest);

	
}