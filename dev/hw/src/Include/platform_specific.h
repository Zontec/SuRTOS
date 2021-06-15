#ifndef __PLATFORM_SPECIFIC_H__
#define __PLATFORM_SPECIFIC_H__

#include "types.h"
#include "serial.h"
#include "macros.h"

#define GET_FUNC_THUMB_ADDRESS(f_n)             ((void*)(((uint32_t)&f_n) & ~1))
#define PUT_FUNC_IN_RAM(f_n, f_s, ram)          memcpy(((uint8_t*)(ram)), GET_FUNC_THUMB_ADDRESS(f_n), f_s)

#define _CALL(f, proto, is_thumb)                   ((proto)((uint8_t*)f + is_thumb))     
#define CALL_THUMB(f, proto)                        _CALL(f, proto, 1)
#define CALL_NOTHUMB(f, proto)                      _CALL(f, proto, 0)  
#define CALL                                        CALL_THUMB                       

#define SERIAL_ENABLE               serial_init
#define PUTC                        serial_write_byte
#define PUTS                        serial_write_string
#define PUTN                        serial_write_number
#define GETC                        serial_read_byte
#define GETS                        serial_read_string
#define GETN                        serial_read_number
#define SERIAL_DISABLE              serial_close

#define FLASH_LOCK                  flash_lock
#define FLASH_WRITE                 flash_write
#define FLASH_READ                  flash_read
#define FLASH_UNLOCK                flash_unlock

#define PRINT_ERROR                 printf
#define PRINT_INFO                  printf

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

#endif /* __PLATFORM_SPECIFIC_H__ */