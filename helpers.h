#ifndef FAT_HELPERS_H
#define FAT_HELPERS_H

#define __STDC_UTF_16__
#include <uchar.h>

int multibyte_to_16bit(char16_t* dest,char* source);
int bit16_to_multibyte(char* dest,char16_t* source);

#endif
