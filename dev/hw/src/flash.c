#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"
#include "memmap.h"

void flash_write(uint8_t *data, uint32_t data_size)
{
    printf("Flash write function\n\r");
    FLASH_Unlock();
    printf("Unlock done\n\r");

    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
        FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    printf("Clear done\n\r");

    printf("%d\r\n", FLASH_EraseSector(FLASH_Sector_4, VoltageRange_1));

	//FLASH_SetLatency( FLASH_Latency_7);
    printf("lat done\r\n");

    printf("unlock done\r\n");
    for (uint32_t i = 0; i < data_size; i++)
    {
        FLASH_ProgramByte((uint32_t)FLASH_SECTOR4_BASE + i, data[i]);
        printf("%x", ((volatile uint8_t*)(FLASH_SECTOR4_BASE))[i]);
    }
    printf("\n\r");
FLASH_Lock();
}
