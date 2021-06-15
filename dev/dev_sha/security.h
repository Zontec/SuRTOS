#ifndef __SECURITY_H__
#define __SECURITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "macros.h"

typedef enum security_status_e
{
    SECURITY_STATUS_OK =                                0x00000000,
    SECURITY_STATUS_FAIL =                              0x0000000A,
    SECURITY_STATUS_FAIL_NOT_IMPLEMENTED =              0x0000000B,
    SECURITY_STATUS_FAIL_MEMORY_ALLOCATION_ERROR =      0x0000000C,
    SECURITY_STATUS_FAIL_NULL_PTR =                     0x0000000D,
    SECURITY_STATUS_FAIL_INCORRECT_FUNCTION_PARAM =     0x0000000E,
} security_status_e;

#define _SECURITY_EXIT                             sec_exit

#define _SECURITY_FUNCTION_RET_VAR                 sec_ret

#define _SECURITY_FUNCTION_BEGIN                   security_status_e _SECURITY_FUNCTION_RET_VAR = \
                                                    SECURITY_STATUS_OK;

#define _SECURITY_FUNCTION_END                     return _SECURITY_FUNCTION_RET_VAR;

#define _SECURITY_CHECK_VALID_NOT_NULL(x)   \
{                                           \
    if((x) == NULL)                         \
    {                                       \
        _SECURITY_FUNCTION_RET_VAR =        \
        SECURITY_STATUS_FAIL_NULL_PTR;      \
        goto _SECURITY_EXIT;                \
    }                                       \
}                                           \

#define _SECURITY_VALID_RES(x)              \
{                                           \
    if(_SECURITY_FUNCTION_RET_VAR = (x)     \
        != SECURITY_STATUS_OK)              \
    {                                       \
        goto _SECURITY_EXIT;                \
    }                                       \
}                                           \

#endif /*__SECURITY_H__*/