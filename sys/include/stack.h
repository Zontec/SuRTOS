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
#ifndef __STACK_H__
#define __STACK_H__

#include "types.h"
#include "platform_specific.h"

#ifndef DEFAULT_STACK_CAPACITY
#define DEFAULT_STACK_CAPACITY (32)
#endif

typedef struct stack_t 
{
    void **ptrs;
    uint32_t size;
    uint32_t capacity;
} stack_t;
/**
 * @brief 
 * 
 * @param stack 
 */
void stack_init(stack_t *stack);
/**
 * @brief 
 * 
 * @param stack 
 * @param data 
 */
void stack_push(stack_t *stack, const void *data);
/**
 * @brief 
 * 
 * @param stack 
 * @return void* 
 */
void *stack_top(const stack_t *stack);
/**
 * @brief 
 * 
 * @param stack 
 * @return void* 
 */
void *stack_pop(stack_t *stack);
/**
 * @brief 
 * 
 * @param stack 
 * @return uint32_t 
 */
uint32_t stack_size(const stack_t *stack);
/**
 * @brief 
 * 
 * @param stack 
 * @return uint32_t 
 */
uint32_t stack_capacity(const stack_t *stack);


#endif /* __STACK_H__ */