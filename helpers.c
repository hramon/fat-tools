#include "fat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>

/*mbrtoc16 cannot be used because Windows doesn't properly check the locale of the multibyte input*/
int multibyte_to_16bit(char16_t** dest,const char* source){

	wchar_t* temp_name;
	int length = mbstowcs(NULL,source, strlen(source));

	temp_name = (wchar_t*)malloc((length + 1)*sizeof(wchar_t));
	mbstowcs(temp_name, source, length + 1);

	/*Windows assigns a length of 2 to whchar_t and other operating systems typically 4*/
	if (sizeof(wchar_t) == 4) {
		int i;
		*dest = (char16_t*)calloc(length + 1, sizeof(char16_t));
		for (i = 0; i < length; i++) {
			/*cast to 2 bytes, throw away the other bytes since longer characters are not supported by FAT*/
			(*dest)[i] = (char16_t)temp_name[i];
		}

		free(temp_name);
	}
	else {
		/*the name is already in the correct format*/
		*dest = temp_name;
	}

	return length;
}

/*c16rtomb is not used because mbrtoc16 wasn't used and to prevent unpredictable outcomes. char16_t is manually converted to wchar_t irrespective of the platform implementation of wchar_t*/
int bit16_to_multibyte(char** dest,const char16_t* source){

#define MAXLENGTH 128

	int length;
	wchar_t* temp_name;

	int i;
	temp_name = (wchar_t*)malloc(sizeof(wchar_t*)*MAXLENGTH);

	i = 0;

	while (source[i] != 0) {
		if (i%MAXLENGTH == 0 && i != 0) {
			temp_name = (wchar_t*)malloc(sizeof(wchar_t*)*(i/MAXLENGTH+1)*MAXLENGTH);
		}

		temp_name[i] = (wchar_t)source[i];
		i++;
	}

	temp_name[i] = 0;

	length = wcstombs(NULL, (const wchar_t*)temp_name,i*2);
	*dest = (char*)malloc((length + 1)*sizeof(char));
	wcstombs(*dest, (const wchar_t*)temp_name,length + 1);

	free(temp_name);

	return length;

}



