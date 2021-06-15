/****************************INFORMATION***********************************
*
*
*
*
**************************************************************************/

/*!
*   @file
*   @brief SHA algorithm header file
*	@author Zontec
*	@version 1.0
*	@date 2021.04.04
*/
#ifndef __STDUTILS_H__
#define __STDUTILS_H__

#include "types.h"
#include "macros.h"

/**
 * @brief 
 * 
 * @param x 
 * @return char 
 */
char to_lower(char x);
/**
 * @brief 
 * 
 * @param x 
 * @return char 
 */
char to_upper(char x);
/**
 * @brief 
 * 
 * @param str 
 * @return char* 
 */
char *lower_string(char *str);
/**
 * @brief 
 * 
 * @param str 
 * @return char* 
 */
char *upper_string(char *str);
/**
 * @brief 
 * 
 * @param dst 
 * @param src 
 * @param len 
 */
void mem_xor(void *dst, const void *src, uint32_t len);
/**
 * @brief 
 * 
 * @param buf1 
 * @param buf2 
 * @param len 
 */
void mem_swap(void *buf1, void *buf2, uint32_t len);
/**
 * @brief 
 * 
 * @param buf1 
 * @param buf2 
 */
void swapU8(uint8_t *buf1, uint8_t *buf2);
/**
 * @brief 
 * 
 * @param buf1 
 * @param buf2 
 */
void swapU16(uint16_t buf1, uint16_t buf2);
/**
 * @brief 
 * 
 * @param buf1 
 * @param buf2 
 */
void swapU32(uint32_t buf1, uint32_t buf2);
/**
 * @brief 
 * 
 * @param buf1 
 * @param buf2 
 */
void swapU64(uint64_t buf1, uint64_t buf2);

#endif /* __STDUTILS_H__ */