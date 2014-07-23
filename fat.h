#ifndef FAT_H
#define FAT_H

#include <stdio.h>

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
	unsigned short DIR_FileSize;
}fat_Directory_Entry;

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

typedef struct _internal_file{
	unsigned int start_cluster;
	unsigned int current_cursor;
	unsigned int current_cluster;
	unsigned int start_directory_entry;
	fat_Directory_Entry file;
}internal_file;

void create_fat(char* filename, fat_type type, unsigned int size);
void read_fat(fat_object* obj, char* filename);
void close_fat(fat_object* obj);
void flush_fat(fat_object* obj);
void copy_file_to_fat(fat_object* obj,char* file_to_copy,char* destination);
internal_file* open_file_fat(fat_object* obj,char* path);
void remove_file_fat(fat_object* obj,char* path);
void write_file_fat(fat_object* obj,internal_file* file,void * buffer, unsigned int size_buffer);
void close_file_fat(fat_object* obj,internal_file* file);
unsigned int find_next_free_dir_entry(fat_object* obj, unsigned int current_directory);
unsigned int find_next_free_cluster(fat_object* obj);

#endif