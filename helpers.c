#include "fat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>


int multibyte_to_16bit(char16_t* dest,char* source){
	char16_t* temp;
	int current_char;
	int maxlength = 128;
	char *ptr, *end;
    int rc;
	mbstate_t state;

	current_char = 0;
	temp = (char16_t*) malloc(sizeof(char16_t)*maxlength);

	char16_t c16;
    ptr = source;
	end = source + strlen(source);

    while(rc = mbrtoc16(temp+current_char, ptr, end - ptr, &state)){   
		if(rc > 0) {
            ptr += rc;
			current_char++;
        }

		if(current_char == maxlength){
			maxlength += 128;
			temp = (char16_t*)realloc(temp,sizeof(char16_t)*maxlength);
		}

    }

	dest = (char16_t*)calloc(sizeof(char16_t),current_char+1);
	memcpy(dest,temp,sizeof(char16_t)*(current_char));
	free(temp);
	return current_char;

}

int bit16_to_multibyte(char* dest,char16_t* source){
	mbstate_t state;
	int rc;
	int maxlength = 128;
	int current_char = 0;
	char* temp;
	char16_t* ptr = source;

	temp = (char*) malloc(sizeof(char)*maxlength);

	while(&ptr != 0){
		rc = c16rtomb(temp+current_char,*ptr,&state);
		current_char += rc;
		ptr++;


		if(current_char > maxlength-2){
			maxlength += 128;
			temp = (char*)realloc(temp,sizeof(char)*maxlength);
		}
	}


	dest = (char*)calloc(sizeof(char),current_char+1);
	memcpy(dest,temp,sizeof(char)*(current_char));
	free(temp);
	return current_char;	
}



