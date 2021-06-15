#include "types.h"

void cli_pbkdf2_handler(char *cmd)
{
    char *pass = strtok(cmd, " ");
    cmd += strlen(pass) + 1;
    char *ssid = strtok(cmd, " ");

    uint8_t out[32];

    pbkdf2_hmac_sha1(pass, strlen(pass), ssid, strlen(ssid), 4096, out, 32);

    for (int i = 0; i < 32; i++)
    {
        printf("%x", out[i]);
    }
    printf("\n\r");
}