

#include "types.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

void cli_pin_handler(char *cmd)
{
    char *port = strtok(cmd, " ");
    cmd += strlen(port) + 1;
    char *num = strtok(cmd, " ");
    cmd += strlen(num) + 1;
    char *state = strtok(cmd, " ");

    uint32_t p;

    if (*port == 'a')
        p = GPIOA;

    if (*port == 'b')
        p = GPIOB;

    if (*port == 'c')
        p = GPIOC;

    if (*port == 'd')
        p = GPIOD;

    GPIO_WriteBit(p, (1) << atoi(num), state[0]-'0');
    
/*
    if (!strcmp(port, "pb12"))
        GPIO_WriteBit(GPIOD, GPIO_Pin_12, state[0] - '0');

    if (!strcmp(port, "pb13"))
        GPIO_WriteBit(GPIOD, GPIO_Pin_12, state[0] - '0');

    if (!strcmp(port, "pb13"))
        GPIO_WriteBit(GPIOD, GPIO_Pin_12, state[0] - '0');

    if (!strcmp(port, "pb13"))
        GPIO_WriteBit(GPIOD, GPIO_Pin_12, state[0] - '0');
        */
}
