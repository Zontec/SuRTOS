

#include "types.h"


void cli_sha1_handler(char *cmd)
{
    char *u = strtok(cmd, " ");
    uint8_t out[20];

    sha1(u, strlen(u), out);

    for( int i = 0; i < 20; i++)
        printf("%x", (uint8_t)out[i]);
    printf("\r\n");
}

void cli_sha256_handler(char *cmd)
{
    char *u = strtok(cmd, " ");
    uint8_t out[32];

    sha256(u, strlen(u), out);
    
    for( int i = 0; i < 32; i++)
        printf("%x", (uint8_t)out[i]);
    printf("\r\n");
}

void cli_sha512_handler(char *cmd)
{
    char *u = strtok(cmd, " ");
    uint8_t out[64];

    sha512(u, strlen(u), out);
    
    for( int i = 0; i < 64; i++)
        printf("%x", (uint8_t)out[i]);
    printf("\r\n");
}