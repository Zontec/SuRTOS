#ifndef __PLATFORM_SPECIFIC_H__
#define __PLATFORM_SPECIFIC_H__

#include "types.h"
#include "serial.h"

#define SERIAL_ENABLE serial_init
#define PUTC serial_write_byte
#define PUTS serial_write_string
#define PUTN serial_write_number
#define SERIAL_DISABLE serial_close

#define PRINT_ERROR     printf

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

#endif /*__PLATFORM_SPECIFIC_H__*/