#include "fat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>


int multibyte_to_16bit(char16_t** dest,char* source){
	char16_t* temp;
	int current_char;
	int maxlength = 128;
	char *ptr, *end;
    int rc;
	mbstate_t state;
	char16_t converted;

	current_char = 0;
	temp = (char16_t*) malloc(sizeof(char16_t)*maxlength);

	char16_t c16;
    ptr = source;
	end = source + strlen(source);

    while(rc = mbrtoc16(&converted, ptr, end - ptr, &state)){   
		if(rc > 0) {
			temp[current_char] = converted;
            ptr += rc;
			current_char++;
        }

		if(current_char == maxlength){
			maxlength += 128;
			temp = (char16_t*)realloc(temp,sizeof(char16_t)*maxlength);
		}

    }

	*dest = (char16_t*)calloc(current_char+1,sizeof(char16_t));
	memcpy(*dest,temp,sizeof(char16_t)*(current_char));
	free(temp);
	return current_char;

}

int bit16_to_multibyte(char** dest,char16_t* source){
	mbstate_t state={0};
	int rc;
	int maxlength = 128;
	int current_char = 0;
	char* temp;
	char16_t* ptr = source;

	temp = (char*) malloc(sizeof(char)*maxlength);

	while(&ptr != 0){
		rc = c16rtomb(temp+current_char,*ptr,&state);
		current_char += rc;
		if(*ptr == 0)
			break;
		ptr++;

		if(current_char > maxlength-2){
			maxlength += 128;
			temp = (char*)realloc(temp,sizeof(char)*maxlength);
		}
	}


	*dest = (char*)calloc(current_char+1,sizeof(char));
	memcpy(*dest,temp,sizeof(char)*(current_char));
	free(temp);
	return current_char;	
}



