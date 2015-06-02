#include "fat.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


unsigned char compare_fat_names(char name1[11],char name2[11]){
	unsigned int i;

	char name1_upper[11];
	char name2_upper[11];

	memcpy(name1_upper,name1,11);
	memcpy(name2_upper,name2,11);
	
	for(i=0;i<11;i++){
		name1_upper[i]=toupper(name1[i]);
		name2_upper[i]=toupper(name2[i]);
	}

	return (memcmp(name1_upper,name2_upper,11)==0);
}

void filename_to_fat_name(char* filename, char* FAT_name){
    char* name;
    unsigned int i;
    char* temp = (char*)malloc(sizeof(char)*(strlen(filename)+1));
    
    strcpy(temp,filename);
    
    for(i=0;i<11;i++){
        FAT_name[i]=' ';
    }
    
    name=strtok(temp,".");
    memcpy(FAT_name,name,strlen(name)<=8?strlen(name):8);
    
    if((name=strtok(NULL,"."))!=NULL){
        memcpy(FAT_name+8,name,strlen(name)<=3?strlen(name):3);
    }
    
    free(temp);
    
}

file_path* split_path(char* path){
    
    char* subpath;
    char** subpaths;
    unsigned int number_of_paths = 0;
    unsigned int current_length = 100;
    file_path* fp=(file_path*)malloc(sizeof(file_path));
    subpaths = (char**)malloc(100*sizeof(char*)); /*create a dummy string array of size 100*/
    
    /*we do not need the root folder in the path, it is implied*/
    if(path[0]=='/')
		path++;
    
    subpath = strtok(path,"/");
    
    while(subpath != NULL){
                
        subpaths[number_of_paths] = (char*)malloc((strlen(subpath)+1)*sizeof(char));
        strcpy(subpaths[number_of_paths],subpath);
        number_of_paths++;
        subpath = strtok(NULL,"/");
        
        if(number_of_paths==current_length){
            current_length += 100;
            subpaths = (char**)realloc(subpaths,current_length*sizeof(char*));
        }
    }
    
    
    fp->number_of_folders = number_of_paths;
    
    fp->folderstructure = (char**)malloc(fp->number_of_folders*sizeof(char*));
    
    /*copy the dummy array to the real structure*/
    memcpy(fp->folderstructure,subpaths,fp->number_of_folders*sizeof(char*));
    free(subpaths);
    
    return fp;
}

void free_file_path(file_path* path){
    int i;
    
    if(path->folderstructure != NULL){
        for(i=0;i<path->number_of_folders;i++){
            free(path->folderstructure[i]);
        }
        free(path->folderstructure);
    }
    
    free(path);
}