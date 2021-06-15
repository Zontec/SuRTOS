#ifndef __BOARD_H__
#define __BOARD_H__

#include "types.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/**
 * @brief 
 * 
 */
typedef enum board_led_e
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,
    LED4 = 3,
} board_led_e;

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