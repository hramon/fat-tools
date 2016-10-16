#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>


void FAT_write_BPB(fat_BPB* bpb,FILE* file){
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

void FAT_read_BPB(fat_BPB* bpb,FILE* file){
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

void FAT_write_FSInfo(fat_FSInfo* info,FILE* file){

	fwrite(&info->FSI_LeadSig,sizeof(info->FSI_LeadSig),1,file);
	fwrite(info->FSI_Reserved1,sizeof(info->FSI_Reserved1),1,file);
	fwrite(&info->FSI_StrucSig,sizeof(info->FSI_StrucSig),1,file);
	fwrite(&info->FSI_Free_Count,sizeof(info->FSI_Free_Count),1,file);
	fwrite(&info->FSI_Nxt_Free,sizeof(info->FSI_Nxt_Free),1,file);
	fwrite(info->FSI_Reserved2,sizeof(info->FSI_Reserved2),1,file);
	fwrite(&info->FSI_TrailSig,sizeof(info->FSI_TrailSig),1,file);
}

void FAT_read_FSInfo(fat_FSInfo* info,FILE* file){

	fread(&info->FSI_LeadSig,sizeof(info->FSI_LeadSig),1,file);
	fread(info->FSI_Reserved1,sizeof(info->FSI_Reserved1),1,file);
	fread(&info->FSI_StrucSig,sizeof(info->FSI_StrucSig),1,file);
	fread(&info->FSI_Free_Count,sizeof(info->FSI_Free_Count),1,file);
	fread(&info->FSI_Nxt_Free,sizeof(info->FSI_Nxt_Free),1,file);
	fread(info->FSI_Reserved2,sizeof(info->FSI_Reserved2),1,file);
	fread(&info->FSI_TrailSig,sizeof(info->FSI_TrailSig),1,file);
}

void FAT_write_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file){
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
		dir++;
	}
	
}

void FAT_read_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file){
	int i;

	for(i=0;i<n_times;i++){
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
		dir++;
	}
}

void FAT_write_Long_Name_Directory_Entry(fat_Long_Name_Directory_entry* long_name,FILE* file){
	
	
	fwrite(&long_name->LDIR_Ord,sizeof(long_name->LDIR_Ord),1,file);
	fwrite(long_name->LDIR_Name1,sizeof(long_name->LDIR_Name1),1,file);
	fwrite(&long_name->LDIR_Attr,sizeof(long_name->LDIR_Attr),1,file);
	fwrite(&long_name->LDIR_Type,sizeof(long_name->LDIR_Type),1,file);
	fwrite(&long_name->LDIR_Chksum,sizeof(long_name->LDIR_Chksum),1,file);
	fwrite(long_name->LDIR_Name2,sizeof(long_name->LDIR_Name2),1,file);
	fwrite(&long_name->LDIR_FstClusLO,sizeof(long_name->LDIR_FstClusLO),1,file);
	fwrite(long_name->LDIR_Name3,sizeof(long_name->LDIR_Name3),1,file);

}

void FAT_read_Long_Name_Directory_Entry(fat_Long_Name_Directory_entry* long_name,FILE* file){

	fread(&long_name->LDIR_Ord,sizeof(long_name->LDIR_Ord),1,file);
	fread(long_name->LDIR_Name1,sizeof(long_name->LDIR_Name1),1,file);
	fread(&long_name->LDIR_Attr,sizeof(long_name->LDIR_Attr),1,file);
	fread(&long_name->LDIR_Type,sizeof(long_name->LDIR_Type),1,file);
	fread(&long_name->LDIR_Chksum,sizeof(long_name->LDIR_Chksum),1,file);
	fread(long_name->LDIR_Name2,sizeof(long_name->LDIR_Name2),1,file);
	fread(&long_name->LDIR_FstClusLO,sizeof(long_name->LDIR_FstClusLO),1,file);
	fread(long_name->LDIR_Name3,sizeof(long_name->LDIR_Name3),1,file);

}

void FAT_create_fat(char* filename, fat_type type, unsigned int size){
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
	FAT_write_BPB(&bpb,file);
	//fwrite(&bpb,sizeof(fat_BPB),1,file);
	fseek(file,bpb.specific_per_fat_type.fat32.BPB_BkBootSec*DEFAULT_SECTOR_SIZE,SEEK_SET);
	FAT_write_BPB(&bpb,file);
	fseek(file,bpb.specific_per_fat_type.fat32.BPB_FSInfo*DEFAULT_SECTOR_SIZE,SEEK_SET);
	//fwrite(&fs_info,sizeof(fat_FSInfo),1,file);
	FAT_write_FSInfo(&fs_info,file);
	
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

void FAT_read_fat(fat_object* obj, char* filename){
	/*open file*/
	obj->file = fopen((const char*)filename,"r+b");

	/*read BPB table*/
	//fread(&(obj->bpb),sizeof(obj->bpb),1,obj->file);
	FAT_read_BPB(&(obj->bpb),obj->file);

	/*read FSInfo*/
	fseek(obj->file,obj->bpb.specific_per_fat_type.fat32.BPB_FSInfo*obj->bpb.BPB_ByestsPerSec,SEEK_SET);
	//fread(&(obj->fs_info),sizeof(obj->fs_info),1,obj->file);
	FAT_read_FSInfo(&(obj->fs_info),obj->file);
	fflush(obj->file);

	/*normally check here what type of fat it is, but for now only fat32 is supported*/
	obj->type = FAT32;

	/*calculate some shortcuts for faster seeking*/
	obj->start_fat = obj->bpb.BPB_RsvdSecCnt*obj->bpb.BPB_ByestsPerSec;
	obj->first_cluster = obj->start_fat + obj->bpb.specific_per_fat_type.fat32.BPB_FATSz32*2*obj->bpb.BPB_ByestsPerSec;
}

void FAT_close_fat(fat_object* obj){
	/*first flush the fat to the file*/
	FAT_flush_fat(obj);
	/*close the file handle*/
	fclose(obj->file);
}

void FAT_flush_fat(fat_object* obj){
	/*write BPB table*/
	fseek(obj->file,0,SEEK_SET);
	//fwrite(&(obj->bpb),sizeof(obj->bpb),1,obj->file);
	FAT_write_BPB(&(obj->bpb),obj->file);

	if(obj->bpb.specific_per_fat_type.fat32.BPB_BkBootSec!=0){
		fseek(obj->file,obj->bpb.specific_per_fat_type.fat32.BPB_BkBootSec*DEFAULT_SECTOR_SIZE,SEEK_SET);
		FAT_write_BPB(&obj->bpb,obj->file);
	}

	/*write FSInfo*/
	fseek(obj->file,obj->bpb.specific_per_fat_type.fat32.BPB_FSInfo*obj->bpb.BPB_ByestsPerSec,SEEK_SET);
	//fwrite(&(obj->fs_info),sizeof(obj->fs_info),1,obj->file);
	FAT_write_FSInfo(&(obj->fs_info),obj->file);
}

unsigned int FAT_find_next_free_dir_entry(fat_object* obj, unsigned int current_directory,unsigned int n){

	unsigned char not_found = 1;
	fat_Directory_Entry* directory = (fat_Directory_Entry*)malloc(sizeof(fat_Directory_Entry)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus);
	
	while(not_found){
		unsigned int i;
		unsigned int max = obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/SIZE_DIRECTORY_ENTRY;
		unsigned int temp_cluster;
		unsigned int count;
		unsigned char counting = 0;

		/*get current directory*/
		fseek(obj->file,FAT_cluster_cursor(obj,current_directory),SEEK_SET);
		FAT_read_Directory_Entry(directory,obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus/SIZE_DIRECTORY_ENTRY,obj->file);
		fflush(obj->file);

		//directory item needs to be consecutive
		count = 0;
		for(i=0;i<max;i++){
			if(directory[i].DIR_Name[0] == DIR_FREE || directory[i].DIR_Name[0] == DIR_FREE_ETC){
				count++;
				counting = 1;
				if (count == n) {

					if (directory[i].DIR_Name[0] == DIR_FREE_ETC) {
						/*take this directory but make the others free etc*/
						if (i != max - 1) {
							directory[i + 1].DIR_Name[0] = DIR_FREE_ETC;
						}
					}

					return FAT_cluster_cursor(obj, current_directory) + (i - count + 1)*SIZE_DIRECTORY_ENTRY;
				}
			}
		}


		/*find next cluster for this directory*/
		fseek(obj->file,obj->start_fat+(current_directory)*4,SEEK_SET);
		fread(&temp_cluster,sizeof(unsigned int),1,obj->file);
		if(temp_cluster >= 0x0FFFFFF8){
			unsigned int next_cluster = FAT_find_next_free_cluster(obj);
			fseek(obj->file,obj->start_fat+(current_directory-2)*sizeof(unsigned int),SEEK_SET);
			fwrite(&next_cluster,sizeof(unsigned int),1,obj->file);
			fflush(obj->file);
		}

		current_directory = temp_cluster;
	}

	return 0;
}

unsigned int FAT_find_next_free_cluster(fat_object* obj){
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

void FAT_date_time(unsigned short* Date,unsigned short* Time){
	time_t t = time(NULL);
	struct tm* time_local = localtime(&t);

	*Date = (time_local->tm_mday | ((time_local->tm_mon + 1)<<5) | ((time_local->tm_year - 80)<<9));
	*Time = (time_local->tm_sec/2 | (time_local->tm_min<<5) | (time_local->tm_hour<<11));
}

void FAT_clear_directory(fat_object* obj, unsigned int current_directory) {
	fat_Directory_Entry entry;
	fseek(obj->file, FAT_cluster_cursor(obj, current_directory), SEEK_SET);

	entry.DIR_Name[0] = DIR_FREE_ETC;

	FAT_write_Directory_Entry(&entry, 1, obj->file);
}
