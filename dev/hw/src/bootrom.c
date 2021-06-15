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
#include "types.h"
#include "memmap.h"
#include "platform_specific.h"
#include "hmac_sha256.h"

extern void os_load();

void read_otp_key(uint8_t key[OTP_KEY_SIZE])
{
    /* Here is otp read function */
    memcpy(key, "1234567812345678", OTP_KEY_SIZE);
}

void get_hash_code(uint8_t *sector_base, uint32_t sector_size, uint8_t *hash)
{
    uint8_t key[OTP_KEY_SIZE];
    read_otp_key(key);
    hmac_sha256(key, OTP_KEY_SIZE, sector_base, sector_size, hash);
}

void check_sectors_magic()
{

}

void check_states()
{

}

void boot_rom_init_hw()
{

}

bool dfu_enabled()
{
    return false;
}

void dfu()
{
    printf("DFU started!\n\r");

}

void boot()
{
    uint8_t hash_code[HMAC_SHA256_HASH_SIZE];

    /*init bootrom required hw*/

    PRINT_INFO("\n\r            uROS            \n\r");
    PRINT_INFO("\n\r\n\r");
    PRINT_INFO("BOOTROM started!\n\r");
    
    /*init rom security*/
    PRINT_INFO("BOOTROM security initialized!\n\r");

    boot_rom_init_hw();

    /*check signatures*/
    printf("Counting hash code of the security sector...\n\r");

   // get_hash_code((uint8_t*)SECURITY_FLASH_SECTOR_BASE, SECURITY_FLASH_SECTOR_SIZE, hash_code);
    //for(int i = 0;i < HMAC_SHA256_HASH_SIZE; i++)
      //  printf("%x", hash_code[i]);
  
    /*check adresses*/
    printf("\n\rCheck secotrs magic...\n\r");

    check_sectors_magic();

    /*check states*/
    printf("Check system flags...\n\r");

    check_states();

    if (!dfu_enabled())
    {
        printf("DFU not active!\n\r");
        printf("Starting bootloader!\n\r");
        os_load();
    }
    else
    {
        printf("DFU active!\n\r");
        dfu();
    }
}