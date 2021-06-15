#ifndef __MACROS_H__
#define __MACROS_H__

#include "types.h"

#define STATIC_ASSERT(condition, comment)       typedef char _static_assert[1 - 2 * !(condition)]

#define UNUSED(x)                               ((void)(0 ? ((x) = (x)) : (x)))


#define ALIGN(x, a)                             __ALIGN_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_MASK(x, mask)                   (((x) + (mask)) & ~(mask))

#define ALIGN4(x)                               ALIGN(x, 4)
#define ALIGN8(x)                               ALIGN(x, 8)

#define IS_LITTLE_ENDIAN()                      (((*(short *)"21") & 0xFF) == '2')
#define IS_BIG_ENDIAN()                         (((*(short *)"21") & 0xFF) == '1')

#define MIN(x, y)                               (((x) > (y)) ? (y) : (x))
#define MAX(x, y)                               (((x) < (y)) ? (y) : (x))

/*check if GCC is used*/
#define NORETURN                                __attribute__((noreturn))
#define NOTNULL                                 __attribute__((nonull))
#define RESTRICT                                __restrict
#define IO                                      volatile
#define IRQ                                     __attribute__((interrupt))
#define INLINE                                  inline

#define U8(var)                                 ((uint8_t)(var))
#define U16(var)                                ((uint16_t)(var))
#define U32(var)                                ((uint32_t)(var))
#define U64(var)                                ((uint64_t)(var))

#define PTR8(var)                               ((int8_t*)(var))
#define PTR16(var)                              ((int16_t*)(var))
#define PTR32(var)                              ((int32_t*)(var))
#define PTR64(var)                              ((int64_t*)(var))

#define UPTR8(var)                              ((uint8_t*)(var))
#define UPTR16(var)                             ((uint16_t*)(var))
#define UPTR32(var)                             ((uint32_t*)(var))
#define UPTR64(var)                             ((uint64_t*)(var))

#define VUPTR8(var)                             ((volatile uint8_t*)(var))
#define VUPTR16(var)                            ((volatile uint16_t*)(var))
#define VUPTR32(var)                            ((volatile uint32_t*)(var))
#define VUPTR64(var)                            ((volatile uint64_t*)(var))

#define VPTR8(var)                              ((volatile int8_t*)(var))
#define VPTR16(var)                             ((volatile int16_t*)(var))
#define VPTR32(var)                             ((volatile int32_t*)(var))
#define VPTR64(var)                             ((volatile int64_t*)(var))

#define REG8(reg)                               (*VUPTR8((reg)))
#define REG16(reg)                              (*VUPTR16((reg)))
#define REG32(reg)                              (*VUPTR32((reg)))
#define REG64(reg)                              (*VUPTR64((reg)))

#define SET_BIT_REG8(reg, bit)                  (REG8((reg)) |= (1 << (bit)))
#define SET_BIT_REG16(reg, bit)                 (REG16((reg)) |= (1 << (bit)))
#define SET_BIT_REG32(reg, bit)                 (REG32((reg)) |= (1 << (bit)))
#define SET_BIT_REG64(reg, bit)                 (REG64((reg)) |= (1 << (bit)))

#define CLR_BIT_REG8(reg, bit)                  (REG8((reg)) &= ~(1 << (bit)))
#define CLR_BIT_REG16(reg, bit)                 (REG16((reg)) &= ~(1 << (bit)))
#define CLR_BIT_REG32(reg, bit)                 (REG32((reg)) &= ~(1 << (bit)))
#define CLR_BIT_REG64(reg, bit)                 (REG64((reg)) &= ~(1 << (bit)))

#define TOGGLE_BIT_REG8(reg, bit)               (REG8((reg)) ^= (1 << (bit)))
#define TOGGLE_BIT_REG16(reg, bit)              (REG16((reg)) ^= (1 << (bit)))
#define TOGGLE_BIT_REG32(reg, bit)              (REG32((reg)) ^= (1 << (bit)))
#define TOGGLE_BIT_REG64(reg, bit)              (REG64((reg)) ^= (1 << (bit)))

#define GET_BIT_REG8(reg, bit)                  ((REG8((reg)) >> (bit)) & 1)
#define GET_BIT_REG16(reg, bit)                 ((REG16((reg)) >> (bit)) & 1)
#define GET_BIT_REG32(reg, bit)                 ((REG32((reg)) >> (bit)) & 1)
#define GET_BIT_REG64(reg, bit)                 ((REG64((reg)) >> (bit)) & 1)

#define PUT_BIT_REG8(reg, bit, pos)             (REG8((reg)) ^= (-(bit) ^ REG8((reg))) & (1 << (pos)))
#define PUT_BIT_REG16(reg, bit, pos)            (REG16((reg)) ^= (-(bit) ^ REG16((reg))) & (1 << (pos)))
#define PUT_BIT_REG32(reg, bit, pos)            (REG32((reg)) ^= (-(bit) ^ REG32((reg))) & (1 << (pos)))
#define PUT_BIT_REG64(reg, bit, pos)            (REG64((reg)) ^= (-(bit) ^ REG64((reg))) & (1 << (pos)))

#define WRITE_REG8(reg, data)                   (REG8((reg)) = (data))
#define WRITE_REG16(reg, data)                  (REG16((reg)) = (data))
#define WRITE_REG32(reg, data)                  (REG32((reg)) = (data))
#define WRITE_REG64(reg, data)                  (REG64((reg)) = (data))

#define READ_REG8(var, reg)                     var = REG8((reg))
#define READ_REG16(var, reg)                    var = REG16((reg))
#define READ_REG32(var, reg)                    var = REG32((reg))
#define READ_REG64(var, reg)                    var = REG64((reg))

#define CLR_REG8(reg)                           REG8((reg)) = 0x00
#define CLR_REG16(reg)                          REG16((reg)) = 0x0000
#define CLR_REG32(reg)                          REG32((reg)) = 0x00000000
#define CLR_REG64(reg)                          REG64((reg)) = 0x0000000000000000

#define SET_REG8(reg)                           REG8((reg)) = 0xFF
#define SET_REG16(reg)                          REG16((reg)) = 0xFFFF
#define SET_REG32(reg)                          REG32((reg)) = 0xFFFFFFFF
#define SET_REG64(reg)                          REG64((reg)) = 0xFFFFFFFFFFFFFFFF

#define GET_BYTE_REG16(reg, byte)               REG16((reg)) = 0xFFFF
#define GET_BYTE_REG32(reg, byte)               REG32((reg)) = 0xFFFFFFFF
#define GET_BYTE_REG64(reg, byte)               REG64((reg)) = 0xFFFFFFFFFFFFFFFF

#define GET_REG_BYTE(var, byte)                 VUPTR8(var)[byte]


#define SET_BIT_U8(var, bit)                    (U8((var)) |= (1 << (bit)))
#define SET_BIT_U16(var, bit)                   (U16((var)) |= (1 << (bit)))
#define SET_BIT_U32(var, bit)                   (U32((var)) |= (1 << (bit)))
#define SET_BIT_U64(var, bit)                   (U64((var)) |= (1 << (bit)))

#define GET_BIT_U8(var, bit)                    (U8((var)) >> ((bit)) & 1)
#define GET_BIT_U16(var, bit)                   (U16((var)) >> ((bit)) & 1)
#define GET_BIT_U32(var, bit)                   (U32((var)) >> ((bit)) & 1)
#define GET_BIT_U64(var, bit)                   (U64((var)) >> ((bit)) & 1)

#define CLR_BIT_U8(var, bit)                    (U8((var)) &= ~(1 << (bit)))
#define CLR_BIT_U16(var, bit)                   (U16((var)) &= ~(1 << (bit)))
#define CLR_BIT_U32(var, bit)                   (U32((var)) &= ~(1 << (bit)))
#define CLR_BIT_U64(var, bit)                   (U64((var)) &= ~(1 << (bit)))

#define TOGGLE_BIT_U8(var, bit)                 (U8((var)) ^= (1 << (bit)))
#define TOGGLE_BIT_U16(var, bit)                (U16((var)) ^= (1 << (bit)))
#define TOGGLE_BIT_U32(var, bit)                (U32((var)) ^= (1 << (bit)))
#define TOGGLE_BIT_U64(var, bit)                (U64((var)) ^= (1 << (bit)))

#define PUT_BIT_U8(var, bit, pos)               (U8((var)) ^= (-(bit) ^ U8((var))) & (1 << (pos)))
#define PUT_BIT_U16(var, bit, pos)              (U16((var)) ^= (-(bit) ^ U16((var))) & (1 << (pos)))
#define PUT_BIT_U32(var, bit, pos)              (U32((var)) ^= (-(bit) ^ U32((var))) & (1 << (pos)))
#define PUT_BIT_U64(var, bit, pos)              (U64((var)) ^= (-(bit) ^ U64((var))) & (1 << (pos)))

#define CLR_U8(var)                             U8((var)) = 0x00
#define CLR_U16(var)                            U16((var)) = 0x0000
#define CLR_U32(var)                            U32((var)) = 0x00000000
#define CLR_U64(var)                            U64((var)) = 0x0000000000000000

#define SET_U8(var)                             U8((var)) = 0xFF
#define SET_U16(var)                            U16((var)) = 0xFFFF
#define SET_U32(var)                            U32((var)) = 0xFFFFFFFF
#define SET_U64(var)                            U64((var)) = 0xFFFFFFFFFFFFFFFF

#define GET_BYTE(var, byte)                     UPTR8(var)[byte]

#ifndef GET_UINT64_BE
#define GET_UINT64_BE(n, b, i)            \
{                                         \
    (n) = (U64((b)[(i) + 0]) << 56)       \
        | (U64((b)[(i) + 1]) << 48)       \
        | (U64((b)[(i) + 2]) << 40)       \
        | (U64((b)[(i) + 3]) << 32)       \
        | (U64((b)[(i) + 4]) << 24)       \
        | (U64((b)[(i) + 5]) << 16)       \
        | (U64((b)[(i) + 6]) << 8)        \
        | (U64((b)[(i) + 7]) << 0);       \
}
#endif 

#ifndef PUT_UINT64_BE
#define PUT_UINT64_BE(n, b, i)          \
{                                       \
    (b)[(i) + 0] = U8((n) >> 56);       \
    (b)[(i) + 1] = U8((n) >> 48);       \
    (b)[(i) + 2] = U8((n) >> 40);       \
    (b)[(i) + 3] = U8((n) >> 32);       \
    (b)[(i) + 4] = U8((n) >> 24);       \
    (b)[(i) + 5] = U8((n) >> 16);       \
    (b)[(i) + 6] = U8((n) >> 8);        \
    (b)[(i) + 7] = U8((n) >> 0);        \
}
#endif

#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i)            \
{                                         \
    (n) = (U32((b)[(i) + 0]) << 24)       \
        | (U32((b)[(i) + 1]) << 16)       \
        | (U32((b)[(i) + 2]) << 8)        \
        | (U32((b)[(i) + 3]) << 0);       \
}
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n, b, i)         \
{                                      \
    (b)[(i) + 0] = U8((n) >> 24);      \
    (b)[(i) + 1] = U8((n) >> 16);      \
    (b)[(i) + 2] = U8((n) >> 8);       \
    (b)[(i) + 3] = U8((n) >> 0);       \
}
#endif


#ifndef LEFT_ROTATE
#define LEFT_ROTATE(x, i) (x) = ((x) << (i)) | ((x) >> ((1 << sizeof(x) + 1) - (i)))
#endif

#endif /* __MACROS_H__ */
