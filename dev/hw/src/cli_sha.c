

#include "types.h"

#include "sha1.h"
#include "sha256.h"
#include "sha512.h"

void cli_sha1_handler(char *cmd)
{
    char *u = cmd;
    uint8_t out[SHA1_HASH_SIZE];

    if (!strcmp(u, "-h"))
    {
        printf("Help for SHA-1 function\n\r");
        printf("    USAGE\n\r");
        printf("sha1 <string to be hashed>\n\r");
        return;
    }

    sha1(u, strlen(u), out);

    for (uint32_t i = 0; i < SHA1_HASH_SIZE; ++i)
        printf("%x", out[i]);
    printf("\r\n");
}

void cli_sha256_handler(char *cmd)
{
    char *u = cmd;
    uint8_t out[SHA256_HASH_SIZE];

    if (!strcmp(u, "-h"))
    {
        printf("Help for SHA-256 function\n\r");
        printf("    USAGE\n\r");
        printf("sha256 <string to be hashed>\n\r");
        return;
    }

    sha256(u, strlen(u), out);

    for (uint32_t i = 0; i < SHA256_HASH_SIZE; ++i)
        printf("%x", out[i]);
    printf("\r\n");
}

void cli_sha512_handler(char *cmd)
{
    char *u = cmd;
    uint8_t out[SHA512_HASH_SIZE];

    if (!strcmp(u, "-h"))
    {
        printf("Help for SHA-512 function\n\r");
        printf("    USAGE\n\r");
        printf("sha512 <string to be hashed>\n\r");
        return;
    }

    sha512(u, strlen(u), out);

    for (uint32_t i = 0; i < SHA512_HASH_SIZE; ++i)
        printf("%x", out[i]);
    printf("\r\n");
}