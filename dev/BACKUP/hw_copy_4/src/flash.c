#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"

#define ACR_BYTE0_ADDRESS           ((uint32_t)0x40023C00) 
#define MY_FLASH_PAGE_ADDR 0x0800FC00
void flash_en()
{
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
printf("%d\r\n", FLASH_EraseSector(FLASH_Sector_11, VoltageRange_4));

	//FLASH_SetLatency( FLASH_Latency_7);
    printf("lat done\r\n");

    printf("unlock done\r\n");

FLASH_ProgramWord((uint32_t)0x08040000, 0xABABABAB);
FLASH_ProgramWord((uint32_t)0x08040004, 0xABABABAB);
FLASH_ProgramWord((uint32_t)0x08040008, 0xABABABAB);
FLASH_Lock();
printf("%p\r\n", *((uint32_t*)0x08040004));
}
