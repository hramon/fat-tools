#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>


void write_BPB(fat_BPB* bpb,FILE* file){
	fwrite(bpb->BS_jmpBoot,sizeof(bpb->BS_jmpBoot),1,file);
	fwrite(bpb->BS_OEMName,sizeof(bpb->BS_OEMName),1,file);
	fwrite(&bpb->BPB_ByestsPerSec,sizeof(bpb->BPB_ByestsPerSec),1,file);
	fwrite(&bpb->BPB_SecPerClus,sizeof(bpb->BPB_SecPerClus),1,file);
	fwrite(&bpb->BPB_RsvdSecCnt,sizeof(bpb->BPB_RsvdSecCnt),1,file);
	fwrite(&bpb->BPB_NumFATs,sizeof(bpb->BPB_NumFATs),1,file);
	fwrite(&bpb->BPB_RootEntCnt,sizeof(bpb->BPB_RootEntCnt),1,file);
	fwrite(&bpb->BPB_TotSec16,sizeof(bpb->BPB_TotSec16),1,file);
	fwrite(&bpb->BPB_Media,sizeof(bpb->BPB_Media),1,file);
	fwrite(&bpb->BPB_FATSz16,sizeof(bpb->BPB_FATSz16),1,file);
	fwrite(&bpb->BPB_SecPerTrk,sizeof(bpb->BPB_SecPerTrk),1,file);
	fwrite(&bpb->BPB_NumHeads,sizeof(bpb->BPB_NumHeads),1,file);
	fwrite(&bpb->BPB_HiddSec,sizeof(bpb->BPB_HiddSec),1,file);
	fwrite(&bpb->BPB_TotSec32,sizeof(bpb->BPB_TotSec32),1,file);

	/*for the moment only fat32 is supported*/

	fwrite(&bpb->specific_per_fat_type.fat32.BPB_FATSz32,sizeof(bpb->specific_per_fat_type.fat32.BPB_FATSz32),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BPB_ExtFlags,sizeof(bpb->specific_per_fat_type.fat32.BPB_ExtFlags),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BPB_FSVer,sizeof(bpb->specific_per_fat_type.fat32.BPB_FSVer),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BPB_RootClus,sizeof(bpb->specific_per_fat_type.fat32.BPB_RootClus),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BPB_FSInfo,sizeof(bpb->specific_per_fat_type.fat32.BPB_FSInfo),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BPB_BkBootSec,sizeof(bpb->specific_per_fat_type.fat32.BPB_BkBootSec),1,file);
	fwrite(bpb->specific_per_fat_type.fat32.BPB_Reserved,sizeof(bpb->specific_per_fat_type.fat32.BPB_Reserved),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BS_DrvNum,sizeof(bpb->specific_per_fat_type.fat32.BS_DrvNum),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BS_Reserved1,sizeof(bpb->specific_per_fat_type.fat32.BS_Reserved1),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BS_BootSig,sizeof(bpb->specific_per_fat_type.fat32.BS_BootSig),1,file);
	fwrite(&bpb->specific_per_fat_type.fat32.BS_VolID,sizeof(bpb->specific_per_fat_type.fat32.BS_VolID),1,file);
	fwrite(bpb->specific_per_fat_type.fat32.BS_VolLab,sizeof(bpb->specific_per_fat_type.fat32.BS_VolLab),1,file);
	fwrite(bpb->specific_per_fat_type.fat32.BS_FilSysType,sizeof(bpb->specific_per_fat_type.fat32.BS_FilSysType),1,file);
}

void read_BPB(fat_BPB* bpb,FILE* file){
	fread(bpb->BS_jmpBoot,sizeof(bpb->BS_jmpBoot),1,file);
	fread(bpb->BS_OEMName,sizeof(bpb->BS_OEMName),1,file);
	fread(&bpb->BPB_ByestsPerSec,sizeof(bpb->BPB_ByestsPerSec),1,file);
	fread(&bpb->BPB_SecPerClus,sizeof(bpb->BPB_SecPerClus),1,file);
	fread(&bpb->BPB_RsvdSecCnt,sizeof(bpb->BPB_RsvdSecCnt),1,file);
	fread(&bpb->BPB_NumFATs,sizeof(bpb->BPB_NumFATs),1,file);
	fread(&bpb->BPB_RootEntCnt,sizeof(bpb->BPB_RootEntCnt),1,file);
	fread(&bpb->BPB_TotSec16,sizeof(bpb->BPB_TotSec16),1,file);
	fread(&bpb->BPB_Media,sizeof(bpb->BPB_Media),1,file);
	fread(&bpb->BPB_FATSz16,sizeof(bpb->BPB_FATSz16),1,file);
	fread(&bpb->BPB_SecPerTrk,sizeof(bpb->BPB_SecPerTrk),1,file);
	fread(&bpb->BPB_NumHeads,sizeof(bpb->BPB_NumHeads),1,file);
	fread(&bpb->BPB_HiddSec,sizeof(bpb->BPB_HiddSec),1,file);
	fread(&bpb->BPB_TotSec32,sizeof(bpb->BPB_TotSec32),1,file);

	/*for the moment only fat32 is supported*/

	fread(&bpb->specific_per_fat_type.fat32.BPB_FATSz32,sizeof(bpb->specific_per_fat_type.fat32.BPB_FATSz32),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BPB_ExtFlags,sizeof(bpb->specific_per_fat_type.fat32.BPB_ExtFlags),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BPB_FSVer,sizeof(bpb->specific_per_fat_type.fat32.BPB_FSVer),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BPB_RootClus,sizeof(bpb->specific_per_fat_type.fat32.BPB_RootClus),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BPB_FSInfo,sizeof(bpb->specific_per_fat_type.fat32.BPB_FSInfo),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BPB_BkBootSec,sizeof(bpb->specific_per_fat_type.fat32.BPB_BkBootSec),1,file);
	fread(bpb->specific_per_fat_type.fat32.BPB_Reserved,sizeof(bpb->specific_per_fat_type.fat32.BPB_Reserved),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BS_DrvNum,sizeof(bpb->specific_per_fat_type.fat32.BS_DrvNum),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BS_Reserved1,sizeof(bpb->specific_per_fat_type.fat32.BS_Reserved1),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BS_BootSig,sizeof(bpb->specific_per_fat_type.fat32.BS_BootSig),1,file);
	fread(&bpb->specific_per_fat_type.fat32.BS_VolID,sizeof(bpb->specific_per_fat_type.fat32.BS_VolID),1,file);
	fread(bpb->specific_per_fat_type.fat32.BS_VolLab,sizeof(bpb->specific_per_fat_type.fat32.BS_VolLab),1,file);
	fread(bpb->specific_per_fat_type.fat32.BS_FilSysType,sizeof(bpb->specific_per_fat_type.fat32.BS_FilSysType),1,file);
}

void write_FSInfo(fat_FSInfo* info,FILE* file){

	fwrite(&info->FSI_LeadSig,sizeof(info->FSI_LeadSig),1,file);
	fwrite(info->FSI_Reserved1,sizeof(info->FSI_Reserved1),1,file);
	fwrite(&info->FSI_StrucSig,sizeof(info->FSI_StrucSig),1,file);
	fwrite(&info->FSI_Free_Count,sizeof(info->FSI_Free_Count),1,file);
	fwrite(&info->FSI_Nxt_Free,sizeof(info->FSI_Nxt_Free),1,file);
	fwrite(info->FSI_Reserved2,sizeof(info->FSI_Reserved2),1,file);
	fwrite(&info->FSI_TrailSig,sizeof(info->FSI_TrailSig),1,file);
}

void read_FSInfo(fat_FSInfo* info,FILE* file){

	fread(&info->FSI_LeadSig,sizeof(info->FSI_LeadSig),1,file);
	fread(info->FSI_Reserved1,sizeof(info->FSI_Reserved1),1,file);
	fread(&info->FSI_StrucSig,sizeof(info->FSI_StrucSig),1,file);
	fread(&info->FSI_Free_Count,sizeof(info->FSI_Free_Count),1,file);
	fread(&info->FSI_Nxt_Free,sizeof(info->FSI_Nxt_Free),1,file);
	fread(info->FSI_Reserved2,sizeof(info->FSI_Reserved2),1,file);
	fread(&info->FSI_TrailSig,sizeof(info->FSI_TrailSig),1,file);
}

void write_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file){
	int i;

	for(i=0;i<n_times;i++){
		fwrite(dir->DIR_Name,sizeof(dir->DIR_Name),1,file);
		fwrite(&dir->DIR_attr,sizeof(dir->DIR_attr),1,file);
		fwrite(&dir->DIR_NTRes,sizeof(dir->DIR_NTRes),1,file);
		fwrite(&dir->DIR_CrtTimeTenth,sizeof(dir->DIR_CrtTimeTenth),1,file);
		fwrite(&dir->DIR_CrtTime,sizeof(dir->DIR_CrtTime),1,file);
		fwrite(&dir->DIR_CrtDate,sizeof(dir->DIR_CrtDate),1,file);
		fwrite(&dir->DIR_LstAccDate,sizeof(dir->DIR_LstAccDate),1,file);
		fwrite(&dir->DIR_FstClusHI,sizeof(dir->DIR_FstClusHI),1,file);
		fwrite(&dir->DIR_WrtTime,sizeof(dir->DIR_WrtTime),1,file);
		fwrite(&dir->DIR_WrtDate,sizeof(dir->DIR_WrtDate),1,file);
		fwrite(&dir->DIR_FstClusLO,sizeof(dir->DIR_FstClusLO),1,file);
		fwrite(&dir->DIR_FileSize,sizeof(dir->DIR_FileSize),1,file);
	}
	
}

void read_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file){
	int i;

	for(i=0;i<n_times;i++){
		dir=dir+i;
		fread(dir->DIR_Name,sizeof(dir->DIR_Name),1,file);
		fread(&dir->DIR_attr,sizeof(dir->DIR_attr),1,file);
		fread(&dir->DIR_NTRes,sizeof(dir->DIR_NTRes),1,file);
		fread(&dir->DIR_CrtTimeTenth,sizeof(dir->DIR_CrtTimeTenth),1,file);
		fread(&dir->DIR_CrtTime,sizeof(dir->DIR_CrtTime),1,file);
		fread(&dir->DIR_CrtDate,sizeof(dir->DIR_CrtDate),1,file);
		fread(&dir->DIR_LstAccDate,sizeof(dir->DIR_LstAccDate),1,file);
		fread(&dir->DIR_FstClusHI,sizeof(dir->DIR_FstClusHI),1,file);
		fread(&dir->DIR_WrtTime,sizeof(dir->DIR_WrtTime),1,file);
		fread(&dir->DIR_WrtDate,sizeof(dir->DIR_WrtDate),1,file);
		fread(&dir->DIR_FstClusLO,sizeof(dir->DIR_FstClusLO),1,file);
		fread(&dir->DIR_FileSize,sizeof(dir->DIR_FileSize),1,file);
	}
}

void create_fat(char* filename, fat_type type, unsigned int size){
	/*for the moment only fat_type.FAT32 is supported*/

	unsigned char buf = 0;
	unsigned int buf2;
	FILE* file;
	unsigned int n_clusters;
	unsigned int cluster_size;
	fat_BPB bpb = {0}; /*initialize on 0*/
	fat_FSInfo fs_info= {0};
	unsigned int i;

	/*size must be a multiple of DEFAULT_SECTOR_SIZE*/
	unsigned int actual_fat_size = size/DEFAULT_SECTOR_SIZE;


	/*first create the raw file (fill with zeros)*/
	file = fopen((const char*)filename, "wb");

	/*for some reason fwrite(&buf,sizeof(buf),size,file) does not work if size is to large
	must be some limint on it*/
	for(i=0;i<size;i++)
		fwrite(&buf,sizeof(buf),1,file);


	/*calculate the number of clusters*/

	bpb.BS_jmpBoot[0]=0xEB;
	bpb.BS_jmpBoot[1]=0x58;
	bpb.BS_jmpBoot[2]=0x90;
	memcpy(bpb.BS_OEMName,"FATTOOL ",8);
	bpb.BPB_ByestsPerSec = DEFAULT_SECTOR_SIZE;
	bpb.BPB_SecPerClus = DEFAULT_SECTORS_PER_CLUSTER;
	bpb.BPB_RsvdSecCnt = 32;/*According to specification, they use 32*/
	bpb.BPB_NumFATs = 2;
	bpb.BPB_RootEntCnt = 0; /*fat32 doesn't use this and it must be 0*/
	bpb.BPB_TotSec16 = 0; /*not for fat32*/
	bpb.BPB_Media = 0xF8; /*fixed, non-removable drive*/
	bpb.BPB_FATSz16 = 0; /*not for fat32*/
	bpb.BPB_TotSec32 = actual_fat_size;

	/*fat32 specific fields*/

	/*calculate the number of sectors the fat will need by solving following equation*/
	/*actual_fat_size-2-2*fatsize_sectors = n_clusters
	 n_clusters*4/DEFAULT_SECTOR_SIZE = fatsize_sectors*/
	n_clusters = (unsigned int)((actual_fat_size-2)/(1+2*4.0/DEFAULT_SECTOR_SIZE));

	bpb.specific_per_fat_type.fat32.BPB_FATSz32 = (n_clusters*4 + DEFAULT_SECTOR_SIZE -1)/DEFAULT_SECTOR_SIZE;
	bpb.specific_per_fat_type.fat32.BPB_ExtFlags = 0x04; /*no mirroring, and fat 0 is the active fat*/
	bpb.specific_per_fat_type.fat32.BPB_FSVer = 0; /*fat32 version 0:0*/
	bpb.specific_per_fat_type.fat32.BPB_RootClus = 0x2; /*there are no clusters 0 and 1 */
	bpb.specific_per_fat_type.fat32.BPB_FSInfo = 1;
	bpb.specific_per_fat_type.fat32.BPB_BkBootSec = 6;
	bpb.specific_per_fat_type.fat32.BS_BootSig = 0x29; /*from fat specification indicate that the following fields are present*/
	bpb.specific_per_fat_type.fat32.BS_VolID = (unsigned int)time(NULL); /*take a unique volume id*/
	memcpy(bpb.specific_per_fat_type.fat32.BS_VolLab,"FAT32TOOL  ",11);
	memcpy(bpb.specific_per_fat_type.fat32.BS_FilSysType,"FAT32   ",8);

	/*make the fs_info structure*/
	fs_info.FSI_LeadSig = FSI_LEAD_SIGNATURE; /*signature to recognize*/
	fs_info.FSI_StrucSig = FSI_STRUCT_SIGNATURE;
	fs_info.FSI_Free_Count = n_clusters-1; /*cluster 2 is used for the root directory*/
	fs_info.FSI_Nxt_Free = 3; /*sectors start at number 2 is the rootCluster*/
	fs_info.FSI_TrailSig = FSI_TRAIL_SIGNATURE;


	/*write the datastructures to the file*/
	rewind(file);
	write_BPB(&bpb,file);
	//fwrite(&bpb,sizeof(fat_BPB),1,file);
	fseek(file,bpb.specific_per_fat_type.fat32.BPB_BkBootSec*DEFAULT_SECTOR_SIZE,SEEK_SET);
	write_BPB(&bpb,file);
	fseek(file,bpb.specific_per_fat_type.fat32.BPB_FSInfo*DEFAULT_SECTOR_SIZE,SEEK_SET);
	//fwrite(&fs_info,sizeof(fat_FSInfo),1,file);
	write_FSInfo(&fs_info,file);
	
	/*add boot signature to the end of sector 0*/
	buf = 0x55;
	fseek(file,510,SEEK_SET);
	fwrite(&buf,1,1,file);
	buf = 0xAA;
	fwrite(&buf,1,1,file);

	/*populate root directory*/
	/*normally the root directory should already be initialized to all zeros*/

	fseek(file,bpb.BPB_RsvdSecCnt*bpb.BPB_ByestsPerSec,SEEK_SET);
	buf2 = 0x0FFFFFF8;
	fwrite(&buf2,sizeof(buf2),1,file);
	buf2 = 0xFFFFFFFF;
	fwrite(&buf2,sizeof(buf2),1,file);
	fwrite(&buf2,sizeof(buf2),1,file);

	/*close the file*/
	fclose(file);
}

void read_fat(fat_object* obj, char* filename){
	/*open file*/
	obj->file = fopen((const char*)filename,"r+b");

	/*read BPB table*/
	//fread(&(obj->bpb),sizeof(obj->bpb),1,obj->file);
	read_BPB(&(obj->bpb),obj->file);

	/*read FSInfo*/
	fseek(obj->file,obj->bpb.specific_per_fat_type.fat32.BPB_FSInfo*obj->bpb.BPB_ByestsPerSec,SEEK_SET);
	//fread(&(obj->fs_info),sizeof(obj->fs_info),1,obj->file);
	read_FSInfo(&(obj->fs_info),obj->file);
	fflush(obj->file);

	/*normally check here what type of fat it is, but for now only fat32 is supported*/
	obj->type = FAT32;

	/*calculate some shortcuts for faster seeking*/
	obj->start_fat = obj->bpb.BPB_RsvdSecCnt*obj->bpb.BPB_ByestsPerSec;
	obj->first_cluster = obj->start_fat + obj->bpb.specific_per_fat_type.fat32.BPB_FATSz32*2*obj->bpb.BPB_ByestsPerSec;
}

void close_fat(fat_object* obj){
	/*first flush the fat to the file*/
	flush_fat(obj);
	/*close the file handle*/
	fclose(obj->file);
}

void flush_fat(fat_object* obj){
	/*write BPB table*/
	fseek(obj->file,0,SEEK_SET);
	//fwrite(&(obj->bpb),sizeof(obj->bpb),1,obj->file);
	write_BPB(&(obj->bpb),obj->file);

	if(obj->bpb.specific_per_fat_type.fat32.BPB_BkBootSec!=0){
		fseek(obj->file,obj->bpb.specific_per_fat_type.fat32.BPB_BkBootSec*DEFAULT_SECTOR_SIZE,SEEK_SET);
		write_BPB(&obj->bpb,obj->file);
	}

	/*write FSInfo*/
	fseek(obj->file,obj->bpb.specific_per_fat_type.fat32.BPB_FSInfo*obj->bpb.BPB_ByestsPerSec,SEEK_SET);
	//fwrite(&(obj->fs_info),sizeof(obj->fs_info),1,obj->file);
	write_FSInfo(&(obj->fs_info),obj->file);
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

internal_file* open_file_fat(fat_object* obj,char* path){
	/*search for the folder or file*/
	/*if the folder does not exist, quit, if the file does not exist, make it*/

	int i;	
	char* subpath;
	unsigned int current_directory = obj->bpb.specific_per_fat_type.fat32.BPB_RootClus;

	/*make the file object*/
	internal_file* file = (internal_file*)calloc(1,sizeof(internal_file));

	/*convert to upper case*/
	for(i=0;i<strlen(path);i++)
		path[i]=toupper(path[i]);

	/*we do not need the root folder in the path, it is implied*/
	if(path[0]=='/')
		path++;

	/*break the path by the / token*/
	subpath = strtok(path,"/");
	while(subpath!=NULL){
		/*construct the correct name*/
		char name[11];
		unsigned int i=0;
		int offset=0;
		unsigned char match = 0;
		unsigned char not_found = 0;

		enum{
			FIND_FOLDER,
			FIND_FILE,
			FIND_BOTH /*first match folder or file*/
		}find_type;

		fat_Directory_Entry* directory = (fat_Directory_Entry*)malloc(sizeof(fat_Directory_Entry)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus);
		unsigned int current_directory_cluster = current_directory;

		while(subpath[i]!='/'&&subpath[i]!='\0'&&offset + i<11){
			if(subpath[i]=='.'){
				/*fill the rest with spaces until the extentionfield*/
				while(offset + i <8){
					name[offset + i] = ' ';
					offset++;
				}
				offset--;
			}else{
				name[offset + i] = subpath[i];
			}

			i++;
		}


		if(subpath[i]=='/'){
			/*we need to find a folder*/
			find_type = FIND_FOLDER;
		}else if(subpath[i]=='\0'){
			/*we need to find a file*/
			find_type = FIND_FILE;
		}else{
			/*the string was to long, just keep the short one*/
			find_type = FIND_BOTH; 
		}

		while(offset + i<11){
			name[offset+i] = ' ';
			i++;
		}


		/*begin the search in current directory*/
		while(!match && !not_found){
			/*get current directory*/
			fseek(obj->file,obj->first_cluster+(current_directory_cluster-2)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus,SEEK_SET);
			//fread(directory,sizeof(fat_Directory_Entry),obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/sizeof(fat_Directory_Entry),obj->file);
			read_Directory_Entry(directory,obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/sizeof(fat_Directory_Entry),obj->file);
			fflush(obj->file);

			for(i=0;i<obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/sizeof(fat_Directory_Entry);i++){
				if(memcmp(directory[i].DIR_Name,name,11)==0){
					/*we found the folder or file*/
					match = 1;
					break;
				}
			}

			if(!match){
				/*find the next cluster in the directory*/
				fseek(obj->file,obj->start_fat+(current_directory_cluster)*4,SEEK_SET);
				fread(&current_directory_cluster,sizeof(unsigned int),1,obj->file);
				fflush(obj->file);
				if(current_directory_cluster >= 0x0FFFFFF8){
					not_found = 1;
				}
			}
		}

		if(match){
			if(find_type == FIND_FILE){
				/*we can stop here*/
				memcpy(&(file->file),&directory[i],sizeof(fat_Directory_Entry));
				file->current_cluster = (directory[i].DIR_FstClusHI<<16)+directory[i].DIR_FstClusLO;
				file->current_cursor = 0;
				file->start_cluster = file->current_cluster;
				file->start_directory_entry = i*sizeof(directory[i])+cluster_cursor(obj,current_directory_cluster);
				free(directory);
				return file;
			}else{
				current_directory = (directory[i].DIR_FstClusHI<<16)+directory[i].DIR_FstClusLO;
			}
		}else if(not_found){
			if(find_type == FIND_FILE){
				/*allocate new here*/
				time_t t = time(NULL);
				struct tm* time = localtime(&t);
				unsigned int new_file = find_next_free_dir_entry(obj,current_directory);

				file->current_cluster = 0;
				file->current_cursor = 0;
				file->start_cluster = file->current_cluster;
				file->start_directory_entry = cluster_cursor(obj,current_directory) + new_file*sizeof(fat_Directory_Entry);

				memcpy(file->file.DIR_Name,name,11);
				file->file.DIR_CrtDate = (time->tm_mday | ((time->tm_mon + 1)<<5) | ((time->tm_year - 80)<<9));
				file->file.DIR_CrtTime = (time->tm_sec/2 | (time->tm_min<<5) | (time->tm_hour<<11));
				file->file.DIR_WrtDate = file->file.DIR_CrtDate;
				file->file.DIR_WrtTime = file->file.DIR_CrtTime;
				file->file.DIR_LstAccDate = file->file.DIR_CrtDate;
                                file->file.DIR_FileSize = 0;
                                file->file.DIR_FstClusHI = file->file.DIR_FstClusLO = 0;

				free(directory);
				return file;
			}else{
				/*we could create a new folder here*/
				free(file);
				free(directory);
				return NULL;
			}
		}

		free(directory);
	}
}

void close_file_fat(fat_object* obj,internal_file* file){
	fseek(obj->file,file->start_directory_entry,SEEK_SET);
	fwrite(&(file->file),sizeof(file->file),1,obj->file);
	fflush(obj->file);
	free(file);
}

unsigned int find_next_free_dir_entry(fat_object* obj, unsigned int current_directory){

	unsigned char not_found = 1;
	unsigned int dir = 0;
	fat_Directory_Entry* directory = (fat_Directory_Entry*)malloc(sizeof(fat_Directory_Entry)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus);
	
	while(not_found){
		unsigned int i;
		unsigned int max = obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/sizeof(fat_Directory_Entry);
		unsigned int temp_cluster;

		/*get current directory*/
		fseek(obj->file,cluster_cursor(obj,current_directory),SEEK_SET);
		//fread(directory,sizeof(fat_Directory_Entry),obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/sizeof(fat_Directory_Entry),obj->file);
		read_Directory_Entry(directory,obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/sizeof(fat_Directory_Entry),obj->file);
		fflush(obj->file);

		for(i=0;i<max;i++){
			if(directory[i].DIR_Name[0] == DIR_FREE){
				/*take this directory*/
				return dir+i;
			}else if(directory[i].DIR_Name[0] == DIR_FREE_ETC){
				/*take this directory but make the others free etc*/
				if(i!=max-1){
					directory[i+1].DIR_Name[0]=DIR_FREE_ETC;
				}

				return dir+i;
			}
		}

		dir+=(max-1);

		/*find next cluster for this directory*/
		fseek(obj->file,obj->start_fat+(current_directory)*4,SEEK_SET);
		fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
		if(temp_cluster >= 0x0FFFFFF8){
			unsigned int next_cluster = find_next_free_cluster(obj);
			fseek(obj->file,obj->start_fat+(current_directory-2)*sizeof(unsigned int),SEEK_SET);
			fwrite(&next_cluster,sizeof(unsigned int),1,obj->file);
			fflush(obj->file);
		}

		current_directory = temp_cluster;
	}

	return 0;
}

unsigned int find_next_free_cluster(fat_object* obj){
	unsigned int next_free = obj->fs_info.FSI_Nxt_Free;
	unsigned int buffer = 0xFFFFFFFF;
	unsigned int current_fat_entry = (next_free)*4 + obj->start_fat;

	fseek(obj->file,current_fat_entry,SEEK_SET);
	fwrite(&buffer,sizeof(buffer),1,obj->file);
	fflush(obj->file);

	

	while(buffer!=0){
		obj->fs_info.FSI_Nxt_Free++;
		fread(&buffer,sizeof(buffer),1,obj->file);		
	}

	return next_free;
}

void write_file_fat(fat_object* obj,internal_file* file,void * buffer, unsigned int size_buffer){
    if(file->file.DIR_FileSize == 0 && size_buffer != 0){
        /*we need to allocate a new cluster for the file*/
        int first_free_cluster = find_next_free_cluster(obj);
        file->current_cluster = first_free_cluster;
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
}

void read_file_fat(fat_object* obj,internal_file* file,void* buffer, unsigned int size_buffer){
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
}

void clear_content_file_fat(fat_object* obj,internal_file* file){
	unsigned int temp_cluster;
	unsigned int buffer = 0xFFFFFFFF;
	file->current_cluster = file -> start_cluster;
	file->current_cursor = 0;

	fseek(obj->file,obj->start_fat+(file->current_cluster)*sizeof(unsigned int),SEEK_SET);
	fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
	fseek(obj->file,-sizeof(unsigned int),SEEK_CUR);
	fwrite(&buffer,sizeof(unsigned int),1,obj->file);
	buffer = 0;

	while(temp_cluster != 0 && temp_cluster < 0x0FFFFFF8){
		if(temp_cluster < obj->fs_info.FSI_Nxt_Free)
			obj->fs_info.FSI_Nxt_Free = temp_cluster;
		fseek(obj->file,obj->start_fat+(temp_cluster-2)*sizeof(unsigned int),SEEK_SET);
		fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
		fseek(obj->file,-sizeof(unsigned int),SEEK_CUR);
		fwrite(&buffer,sizeof(unsigned int),1,obj->file);
	}

	/*if(temp_cluster >= 0x0FFFFFF8){
		fseek(obj->file,obj->start_fat+(temp_cluster-2)*sizeof(unsigned int),SEEK_SET);
		fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
	}*/

	file->file.DIR_FileSize = 0;
}
