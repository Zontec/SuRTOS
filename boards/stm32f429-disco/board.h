#ifndef __BOARD_H__
#define __BOARD_H__

#include "types.h"

typedef enum board_led_e
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,
    LED4 = 3,
};

/**
 * @brief 
 * 
 * 
 */
void board_init();
/**
 * @brief 
 * 
 * @param num 
 */
void led_on(board_led_e num);
/**
 * @brief 
 * 
 * @param num 
 */
void led_off(board_led_e num);
/**
 * @brief 
 * 
 * @param num 
 */
void led_toggle(board_led_e num);

#endif /* __BOARD_H__ */