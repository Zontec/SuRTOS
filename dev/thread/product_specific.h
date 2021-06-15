#ifndef __PRODUCT_SPECIFIC_H__
#define __PRODUCT_SPECIFIC_H__

#include "types.h"

void *memset(void *memptr, int32_t val, uint32_t len);

void *memcpy(void *dest, const void *source, uint32_t len);

int32_t memcmp(const void *buf1, const void *buf2, uint32_t len);


void *malloc(uint32_t len);

void *calloc(uint32_t len, uint32_t type_size);

void *realloc(const void* ptr, uint32_t len);

void free(void *ptr);


uint32_t strlen(const char *str);

int32_t strcmp(const char *str1, const char *str2);

char *strcpy(char *dst, const char *src);

char *strcat(char *dst, const char *src);

#endif /*__PRODUCT_SPECIFIC_H__*/