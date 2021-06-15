#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#define RAM_BEGIN_ADRESS                    (0x20000000)
#define RAM_END_ADRESS                      (0x20020000)

#define HEAP_BASE                           (0x20001000)
#define HEAP_SIZE                           (0x00002000)

#define EXT_RAM_BEGIN_ADRESS                (0x60000000)
#define EXT_RAM_BEGIN_ADRESS                (0xFFFFFFFF)

#define FLASH_BEGIN_ADRESS                  (0x08000000)
#define FLASH_END_ADRESS                    (0x080FFFFF)

#define OTP_BEGIN_ADRESS                    (0x1FFF0000)
#define OTP_END_ADRESS                      (0x1FFF7A00)

#define OPTION_BYTES_BEGIN_ADRESS           (0x1FFFC000)
#define OPTION_BYTES_END_ADRESS             (0x1FFFC000)

#endif /*__MEMMAP_H__*/