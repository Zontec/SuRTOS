#include "types.h"
#include "cli.h"

void cli_pbkdf2_handler(char *cmd)
{
    uint8_t out[64];

    if (!strcmp(cmd, "-h"))
    {
        PRINTF("Help for PBKDF2 function\n\r");
        PRINTF("    USAGE\n\r");
        PRINTF("pbkdf2 <hmac hash type> <password> <salt>\n\r");
        PRINTF("    -hash types allowed: sha1, sha256, sha512\n\r");
        return;
    }

    char *hmac_type = strtok(cmd, " ");
    if (hmac_type == NULL)
    {
        PRINTF("Incorrect input: hash type is not present\n\r");
    }

    char *pass = strtok(cmd, " ");
    cmd += strlen(pass) + 1;
    char *ssid = strtok(cmd, " ");


    pbkdf2_hmac_sha1(pass, strlen(pass), ssid, strlen(ssid), 4096, out, 32);

    for (int i = 0; i < 32; ++i)
    {
        PRINTF("%x", out[i]);
    }
    PRINTF("\n\r");
}