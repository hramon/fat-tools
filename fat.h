#ifndef FAT_H
#define FAT_H

#include <stdio.h>
#include <stdint.h>
#include "fat_path.h"

#define DEFAULT_SECTOR_SIZE 512
#define DEFAULT_SECTORS_PER_CLUSTER 1
#define MAX_CLUSTER_SIZE 128
#define FSI_LEAD_SIGNATURE 0x41615252
#define FSI_STRUCT_SIGNATURE 0x61417272
#define FSI_TRAIL_SIGNATURE 0xAA550000


typedef struct _fat_BPB{
	unsigned char BS_jmpBoot[3];
	char BS_OEMName[8];
	unsigned short BPB_ByestsPerSec;
	unsigned char BPB_SecPerClus;
	unsigned short BPB_RsvdSecCnt;
	unsigned char BPB_NumFATs;
	unsigned short BPB_RootEntCnt;
	unsigned short BPB_TotSec16;
	unsigned char BPB_Media;
	unsigned short BPB_FATSz16;
	unsigned short BPB_SecPerTrk;
	unsigned short BPB_NumHeads;
	unsigned int BPB_HiddSec;
	unsigned int BPB_TotSec32;
	union{
		struct{
			unsigned char BS_DrvNum;
			unsigned char BS_Reserved1;
			unsigned char BS_BootSig;
			unsigned int BS_VolID;
			char BS_VolLab[11];
			char BS_FilSysType[8];
		}fat12_16;
		struct{
			unsigned int BPB_FATSz32;
			unsigned short BPB_ExtFlags;
			unsigned short BPB_FSVer;
			unsigned int BPB_RootClus;
			unsigned short BPB_FSInfo;
			unsigned short BPB_BkBootSec;
			unsigned char BPB_Reserved[12];
			unsigned char BS_DrvNum;
			unsigned char BS_Reserved1;
			unsigned char BS_BootSig;
			unsigned int BS_VolID;
			char BS_VolLab[11];
			char BS_FilSysType[8];
		}fat32;
	}specific_per_fat_type;
}fat_BPB;

typedef struct _fat_FSInfo{
	unsigned int FSI_LeadSig;
	unsigned char FSI_Reserved1[480];
	unsigned int FSI_StrucSig;
	unsigned int FSI_Free_Count;
	unsigned int FSI_Nxt_Free;
	unsigned char FSI_Reserved2[12];
	unsigned int FSI_TrailSig;
}fat_FSInfo;

typedef struct _fat_Directory_Entry{
	char DIR_Name[11];
	unsigned char DIR_attr;
	unsigned char DIR_NTRes;
	unsigned char DIR_CrtTimeTenth;
	unsigned short DIR_CrtTime;
	unsigned short DIR_CrtDate;
	unsigned short DIR_LstAccDate;
	unsigned short DIR_FstClusHI;
	unsigned short DIR_WrtTime;
	unsigned short DIR_WrtDate;
	unsigned short DIR_FstClusLO;
	unsigned int DIR_FileSize;
}fat_Directory_Entry;

#define SIZE_DIRECTORY_ENTRY 32

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

#define DIR_FREE 0xE5
#define DIR_FREE_ETC 0x00 /*dir free and following*/

#define FAT_BPB_SIZE (sizeof(fat_BPB))

typedef enum _fat_type{
	NOT_SPECIFIED = 0,
	FAT12,
	FAT16,
	FAT32
}fat_type;

typedef struct _fat_object{
	fat_type type;
	fat_BPB bpb;
	fat_FSInfo fs_info;
	FILE* file;
	unsigned int start_fat;
	unsigned int first_cluster;
}fat_object;

/*Following read and write functions are to write the structs to the disk. Normal writing cannot be performed due to struct alignment*/
void write_BPB(fat_BPB* bpb,FILE* file);
void read_BPB(fat_BPB* bpb,FILE* file);
void write_FSInfo(fat_FSInfo* info,FILE* file);
void read_FSInfo(fat_FSInfo* info,FILE* file);
void write_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file);
void read_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file);

/*some defines for searching in the raw FAT structure*/
#define cluster_cursor(obj,cluster) (obj->first_cluster+(cluster-2)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus)


/*editing the fat*/
void create_fat(char* filename, fat_type type, unsigned int size);
void read_fat(fat_object* obj, char* filename);
void close_fat(fat_object* obj);
void flush_fat(fat_object* obj);
unsigned int find_next_free_dir_entry(fat_object* obj, unsigned int current_directory);
unsigned int find_next_free_cluster(fat_object* obj);
void make_dir_fat(fat_object* obj, char* path_directory);
unsigned char find_file_in_directory(fat_object* obj, char name[11], unsigned int* index, unsigned int* cluster_directory, unsigned int* fat_Directory_Entry); /*returns 0 if not found, 1 if found*/
void date_time(unsigned short* date,unsigned short* time);

#endif