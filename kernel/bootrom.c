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
    //hmac_sha256(key, OTP_KEY_SIZE, sector_base, sector_size, hash);
}

void check_sectors_magic()
{

}

void check_states()
{

}

void boot_rom_init_security()
{

}

void boot()
{

    PRINTF("\n\r            uROS            \n\r");
    PRINTF("\n\r\n\r");
    PRINTF("BOOTROM started!\n\r");
    
    /* Init ROM security */
    PRINTF("BOOTROM security initialization...!\n\r");
    boot_rom_init_security();

    /*check adresses*/
    PRINTF("\n\rCheck secotrs magic...\n\r");
    check_sectors_magic();

    /*check states*/
    PRINTF("Check system flags...\n\r");
    check_states();

    if (!dfu_is_active())
    {
        PRINTF("DFU not active!\n\r");
        PRINTF("Starting bootloader!\n\r");
        os_load();
    }
    else
    {
        PRINTF("DFU active!\n\r");
        dfu_start();
    }
}