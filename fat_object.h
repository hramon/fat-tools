#ifndef FAT_OBJECT_H
#define FAT_OBJECT_H

#include <stdio.h>
#include <stdint.h>

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

typedef struct _fat_Long_Name_Directory_Entry{
	unsigned char LDIR_Ord;
	unsigned short LDIR_Name1[5];
	unsigned char LDIR_Attr;
	unsigned char LDIR_Type;
	unsigned char LDIR_Chksum;
	unsigned short LDIR_Name2[6];
	unsigned short LDIR_FstClusLO;
	unsigned short LDIR_Name3[2];
}fat_Long_Name_Directory_entry;

#define SIZE_DIRECTORY_ENTRY 32

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
#define LAST_LONG_ENTRY 0x40
#define REVERSE_LAST_LONG_ENTRY 0x3f

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
void FAT_write_BPB(fat_BPB* bpb,FILE* file);
void FAT_read_BPB(fat_BPB* bpb,FILE* file);
void FAT_write_FSInfo(fat_FSInfo* info,FILE* file);
void FAT_read_FSInfo(fat_FSInfo* info,FILE* file);
void FAT_write_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file);
void FAT_read_Directory_Entry(fat_Directory_Entry* dir,unsigned int n_times,FILE* file);
void FAT_write_Long_Name_Directory_Entry(fat_Long_Name_Directory_entry* long_name,FILE* file);
void FAT_read_Long_Name_Directory_Entry(fat_Long_Name_Directory_entry* long_name,FILE* file);

/*some defines for searching in the raw FAT structure*/
#define FAT_cluster_cursor(obj,cluster) (obj->first_cluster+(cluster-2)*obj->bpb.BPB_ByestsPerSec*obj->bpb.BPB_SecPerClus)


/*editing the fat*/
void FAT_create_fat(char* filename, fat_type type, unsigned int size);
void FAT_read_fat(fat_object* obj, char* filename);
void FAT_close_fat(fat_object* obj);
void FAT_flush_fat(fat_object* obj);
unsigned int FAT_find_next_free_dir_entry(fat_object* obj, unsigned int current_directory);
unsigned int FAT_find_next_free_cluster(fat_object* obj);
void FAT_date_time(unsigned short* date,unsigned short* time);

#endif