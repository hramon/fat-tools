#include "fat.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void FAT_remove_file(fat_object* obj,char* path){
	
	internal_file* file = FAT_open_file(obj,path);
	FAT_clear_content_file(obj,file);

	file->file.short_name.DIR_Name[0] = DIR_FREE;

	FAT_close_file(obj,file);
}

void FAT_copy_file(fat_object* obj,char* source,char* destination){

	internal_file* source_file;
	internal_file* destination_file;

	source_file = FAT_open_file(obj,source);
	destination_file = FAT_open_file(obj,destination);

	FAT_clear_content_file(obj,destination_file);
	
	/*write the file to the disk*/
	while(!FAT_eof(source_file)){
		unsigned char buffer;
		FAT_read_file(obj,source_file,&buffer,sizeof(buffer));
		FAT_write_file(obj,destination_file,&buffer,sizeof(buffer));
	}

	FAT_close_file(obj,destination_file);
	FAT_close_file(obj,source_file);
}

void FAT_clear_content_file(fat_object* obj,internal_file* file){
    
	unsigned int temp_cluster;
    unsigned int buffer = 0;

    if(file->file.short_name.DIR_FileSize == 0)
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

    file->file.short_name.DIR_FileSize = 0;
    file->start_cluster = file->current_cluster = 0;
}

void FAT_write_file(fat_object* obj,internal_file* file,void * buffer, unsigned int size_buffer){
    if(file->file.short_name.DIR_FileSize == 0 && size_buffer != 0){
        /*we need to allocate a new cluster for the file*/
        int first_free_cluster = FAT_find_next_free_cluster(obj);
        file->current_cluster = first_free_cluster;
        file->start_cluster = first_free_cluster;
        file->file.short_name.DIR_FstClusLO = first_free_cluster & 0x0000FFFF;
        file->file.short_name.DIR_FstClusHI = first_free_cluster & 0xFFFF0000;
    }else if(size_buffer == 0){
        return;
    }
    
    fseek(obj->file,FAT_cluster_cursor(obj,file->current_cluster)+file->current_cursor,SEEK_SET);
    if(size_buffer + file->current_cursor <= (unsigned int)obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus){
            fwrite(buffer,size_buffer,1,obj->file);
            fflush(obj->file);
            file->current_cursor+=size_buffer;
            file->file.short_name.DIR_FileSize+=size_buffer;
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
                    unsigned int next_cluster = FAT_find_next_free_cluster(obj);
                    fseek(obj->file,obj->start_fat+(file->current_cluster)*sizeof(unsigned int),SEEK_SET);
                    fwrite(&next_cluster,sizeof(unsigned int),1,obj->file);
                    fflush(obj->file);
                    temp_cluster = next_cluster;
                    obj->fs_info.FSI_Free_Count--;
            }
            file->current_cluster = temp_cluster;
            file->file.short_name.DIR_FileSize+=before;
            file->current_cursor = 0;
            FAT_write_file(obj,file,((unsigned char*)buffer)+before,rest);
    }

	FAT_date_time(&(file->file.short_name.DIR_WrtDate),&(file->file.short_name.DIR_WrtTime));
	FAT_date_time(&(file->file.short_name.DIR_LstAccDate),&(file->file.short_name.DIR_WrtTime));
}

void FAT_read_file(fat_object* obj,internal_file* file,void* buffer, unsigned int size_buffer){
	unsigned short dummy;
	if(file->current_total_cursor + size_buffer <= file->file.short_name.DIR_FileSize){
		fseek(obj->file,FAT_cluster_cursor(obj,file->current_cluster)+file->current_cursor,SEEK_SET);
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
			FAT_read_file(obj,file,((unsigned char*)buffer)+before,rest);
		}
	}else if(!FAT_eof(file)){
		FAT_read_file(obj,file,buffer,file->file.short_name.DIR_FileSize-file->current_total_cursor);
	}

	FAT_date_time(&(file->file.short_name.DIR_LstAccDate),&(dummy));
}

internal_file* FAT_open_file(fat_object* obj,char* path){
    /*search for the file*/
    /*if the folder does not exist, quit, if the file does not exist, make it*/

    int i;	
    file_path* fp = NULL;
    unsigned int current_directory = obj->bpb.specific_per_fat_type.fat32.BPB_RootClus;

    /*make the file object*/
    internal_file* file = (internal_file*)calloc(1,sizeof(internal_file));


    fp = FAT_split_path(path);

    for(i=0;i<fp->number_of_folders;i++){
        char name[11];
        unsigned int index = 0;
        unsigned char found = 0;
		//fat_Directory_Entry* directory = NULL;
		//directory_item item;
		unsigned int current_directory_cluster = 0;

        enum{
                FIND_FOLDER,
                FIND_FILE
        }find_type;


        if(i==fp->number_of_folders-1)
            find_type = FIND_FILE;
        else
            find_type = FIND_FOLDER;

        FAT_filename_to_fat_name(fp->folderstructure[i],name);

        //directory = (fat_Directory_Entry*)malloc(sizeof(fat_Directory_Entry)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus);
        current_directory_cluster = current_directory;

		/*search for the name in the directory*/
		found=FAT_find_file_in_directory(obj, fp->folderstructure[i],&index,&current_directory_cluster,&file->file);

		if(found){
			if(find_type == FIND_FILE){
                /*we can stop here*/
                /*memcpy(&(file->file), &file->file.short_name,SIZE_DIRECTORY_ENTRY);*/
                file->current_cluster = (file->file.short_name.DIR_FstClusHI<<16)+ file->file.short_name.DIR_FstClusLO;
                file->current_cursor = 0;
                file->start_cluster = file->current_cluster;
                file->start_directory_entry = index*SIZE_DIRECTORY_ENTRY+FAT_cluster_cursor(obj,current_directory_cluster);
				FAT_free_file_path(fp);
                return file;
            }else{
                current_directory = (file->file.short_name.DIR_FstClusHI<<16)+ file->file.short_name.DIR_FstClusLO;
            }
		}else{
			if(find_type == FIND_FILE){
                /*allocate new here*/
                time_t t = time(NULL);
                struct tm* time = localtime(&t);
				unsigned int new_file;
				FAT_create_directory_item(&file->file, fp->folderstructure[i]);
				new_file = FAT_find_next_free_dir_entry(obj, current_directory, file->file.long_name_entry_length);

                file->current_cluster = 0;
                file->start_cluster = 0;
                file->current_cursor = 0;
                file->start_directory_entry = new_file;

                memcpy(file->file.short_name.DIR_Name,name,11);
				FAT_date_time(&(file->file.short_name.DIR_CrtDate),&(file->file.short_name.DIR_CrtTime));
                file->file.short_name.DIR_WrtDate = file->file.short_name.DIR_CrtDate;
                file->file.short_name.DIR_WrtTime = file->file.short_name.DIR_CrtTime;
                file->file.short_name.DIR_LstAccDate = file->file.short_name.DIR_CrtDate;
                file->file.short_name.DIR_FileSize = 0;
                file->file.short_name.DIR_FstClusHI = file->file.short_name.DIR_FstClusLO = 0;

				FAT_free_file_path(fp);
                return file;
            }else{
                /*we could create a new folder here*/
                free(file);
				FAT_free_file_path(fp);
                return NULL;
            }
		}

    }

	FAT_free_file_path(fp);
    
    return NULL;
}

void FAT_close_file(fat_object* obj,internal_file* file){
	fseek(obj->file,file->start_directory_entry,SEEK_SET);
	FAT_write_directory_item(obj,&(file->file));
	fflush(obj->file);
	free(file);
}

void FAT_copy_file_from_fat(fat_object* obj,char* file_to_copy,char* destination){
	/*open the file to copy*/
	internal_file* source;
	FILE* file = fopen((const char*)destination,"wb");

	/*open source handle*/
	source = FAT_open_file(obj, file_to_copy);


	/*write the file to the disk*/
	while(!FAT_eof(source)){
		unsigned char buffer;
		FAT_read_file(obj,source,&buffer,sizeof(buffer));
		fwrite(&buffer,sizeof(buffer),1,file);
	}


	/*close the file*/
	FAT_close_file(obj,source);
	fclose(file);
}

void FAT_copy_file_to_fat(fat_object* obj,char* file_to_copy,char* destination){
	/*open the file to copy*/
	internal_file* dest;
	unsigned char buffer;
	FILE* file = fopen((const char*)file_to_copy,"rb");	

	/*open destination handle*/
	dest = FAT_open_file(obj, destination);

	FAT_clear_content_file(obj,dest);

	/*write the file to the disk*/
	while(fread(&buffer,sizeof(buffer),1,file)){
		FAT_write_file(obj,dest,&buffer,sizeof(buffer));
	}


	/*close the file*/
	FAT_close_file(obj,dest);

	
}