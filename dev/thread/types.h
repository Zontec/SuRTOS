#ifndef __TYPES_H__
#define __TYPES_H__


#ifndef NULL
#define NULL ((void*)(0x00000000))
#endif

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef long int int32_t;

typedef uint32_t size_t;
typedef unsigned char bool;

#ifndef BOOL
#define BOOL    bool
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#ifndef FALSE
#define FALSE    (0)
#endif

#endif /*__TYPES_H__*/