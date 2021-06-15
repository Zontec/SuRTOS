#include "types.h"

void cli_pbkdf2_handler(char *cmd)
{
    uint8_t out[64];

    if (!strcmp(cmd, "-h"))
    {
        printf("Help for PBKDF2 function\n\r");
        printf("    USAGE\n\r");
        printf("pbkdf2 <hmac hash type> <password> <salt>\n\r");
        printf("    -hash types allowed: sha1, sha256, sha512\n\r");
        return;
    }

    char *hmac_type = strtok(cmd, " ");
    if (hmac_type == NULL)
    {
        printf("Incorrect input: hash type is not present\n\r");
    }

    char *pass = strtok(cmd, " ");
    cmd += strlen(pass) + 1;
    char *ssid = strtok(cmd, " ");


    pbkdf2_hmac_sha1(pass, strlen(pass), ssid, strlen(ssid), 4096, out, 32);

    for (int i = 0; i < 32; ++i)
    {
        printf("%x", out[i]);
    }
    printf("\n\r");
}