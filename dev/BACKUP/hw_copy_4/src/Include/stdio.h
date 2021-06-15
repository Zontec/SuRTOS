#ifndef __STDIO_H__
#define __STDIO_H__

#include "platform_specific.h"
#include "macros.h"

#ifndef SERIAL_ENABLE
#error "SERIAL_ENABLE not implemented!"
#endif

#ifndef SERIAL_DISABLE
#error "SERIAL_DISABLE not implemented!"
#endif

#ifndef PUTC
#error "PUTC not implemented!"
#endif

#ifndef PUTS
#error "PUTC not implemented!"
#endif

#ifndef PUTN
#error "PUTC not implemented!"
#endif

#ifndef GETC
#error "GETC not implemented!"
#endif

#ifndef GETS
#error "GETS not implemented!"
#endif

#ifndef GETN
#error "GETN not implemented!"
#endif

int printf(const char* format, ...);

int scanf(const char* format, ...);

#endif /* __STDIO_H__ */