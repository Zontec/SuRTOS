#include "board.h"

typedef struct led_t
{
    uint32_t PORT;
    uint32_t PIN;
} led_t;

static const led_t leds[] = 
{
    {GPIOA, GPIO_Pin_6},
    {GPIOA, GPIO_Pin_7},
};

static uint32_t num_leds = 0;

void board_init()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;

    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);
    
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