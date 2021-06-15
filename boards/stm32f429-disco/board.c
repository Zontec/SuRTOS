#include "board.h"

typedef struct led_t
{
    uitn32_t PORT;
    uitn32_t PIN;
};

led_t leds[] = 
{
    {GPIOD, GPIO_Pin_12},
    {GPIOD, GPIO_Pin_13},
    {GPIOD, GPIO_Pin_14},
    {GPIOD, GPIO_Pin_15},
};

static uint32_t num_leds = 0;

void board_init()
{
    num_leds = sizeof(leds) / sizeof(led_t);
}

void led_on(board_led_e num)
{
    GPIO_WriteBit(leds[num].PORT, leds[num].PIN, 1);
}

void led_off(board_led_e num)
{
    GPIO_WriteBit(leds[num].PORT, leds[num].PIN, 0);
}

void led_toggle(board_led_e num)
{
    GPIO_ToggleBits(leds[num].PORT, leds[num].PIN);
}