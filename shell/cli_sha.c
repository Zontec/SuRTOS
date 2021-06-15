

#include "types.h"
#include "cli.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"

void cli_sha1_handler(char *cmd)
{
    char *u = cmd;
    uint8_t out[SHA1_HASH_SIZE];

    if (!strcmp(u, "-h"))
    {
        PRINTF("Help for SHA-1 function\n\r");
        PRINTF("    USAGE\n\r");
        PRINTF("sha1 <string to be hashed>\n\r");
        return;
    }

    sha1(u, strlen(u), out);

    for (uint32_t i = 0; i < SHA1_HASH_SIZE; ++i)
        PRINTF("%x", out[i]);
    PRINTF("\n\r");
}

void cli_sha256_handler(char *cmd)
{
    char *u = cmd;
    uint8_t out[SHA256_HASH_SIZE];

    if (!strcmp(u, "-h"))
    {
        PRINTF("Help for SHA-256 function\n\r");
        PRINTF("    USAGE\n\r");
        PRINTF("sha256 <string to be hashed>\n\r");
        return;
    }

    sha256(u, strlen(u), out);

    for (uint32_t i = 0; i < SHA256_HASH_SIZE; ++i)
        PRINTF("%x", out[i]);
    PRINTF("\n\r");
}

void cli_sha512_handler(char *cmd)
{
    char *u = cmd;
    uint8_t out[SHA512_HASH_SIZE];

    if (!strcmp(u, "-h"))
    {
        PRINTF("Help for SHA-512 function\n\r");
        PRINTF("    USAGE\n\r");
        PRINTF("sha512 <string to be hashed>\n\r");
        return;
    }

    sha512(u, strlen(u), out);

    for (uint32_t i = 0; i < SHA512_HASH_SIZE; ++i)
        PRINTF("%x", out[i]);
    PRINTF("\n\r");
}