#ifndef __TYPES_H__
#define __TYPES_H__

#include "config.h"

#ifndef USE_STD_TYPES

#ifndef NULL
#define NULL ((void*)(0x00000000))
#endif /*NULL*/

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef long int int32_t;

typedef uint32_t size_t;
typedef unsigned char bool;

#else

#include <stdint.h>
#include <stdbool.h>

#endif /*USE_STD_TYPES*/

#ifndef BOOL
#define BOOL    bool
#endif /*BOOL*/

#ifndef TRUE
#define TRUE    (1)
#endif /*TRUE*/

#ifndef FALSE
#define FALSE    (0)
#endif /*FALSE*/

#endif /*__TYPES_H__*/