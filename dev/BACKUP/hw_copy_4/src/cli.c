#include "cli.h"

#define MAX_CLI_COMMAND_SIZE        (10)

typedef void(*handler)(char *);

static const char _cli_commands[][MAX_CLI_COMMAND_SIZE] = 
{
    "help",
    "sha1",
    "sha256",
    "sha512",
    "pin",
    "pbkdf2",
};
extern void cli_sha1_handler(char *cmd);
extern void cli_sha256_handler(char *cmd);
extern void cli_sha512_handler(char *cmd);
extern void cli_pin_handler(char *cmd);
extern void cli_pbkdf2_handler(char *cmd);



static const handler _cli_commands_handlers[] = 
{
    NULL,
    cli_sha1_handler,
    cli_sha256_handler,
    cli_sha512_handler,
    cli_pin_handler,
    cli_pbkdf2_handler,
};

static int32_t _cli_find_cmd_index(char *cmd)
{
    char *ptr = strtok(cmd, " ");
    int32_t ret = -1;

    for (uint32_t i = 0; i < sizeof(_cli_commands) / MAX_CLI_COMMAND_SIZE; ++i)
    {
        if (!strcmp(ptr, _cli_commands[i]))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

static void _cli_unknown_command(char *cmd)
{
    printf("Unknown command or symbal: '%s'\n\r", cmd);
}


void cli_handler(char *cmd)
{
    uint32_t t = _cli_find_cmd_index(cmd);

    if (t == -1)
    {
        _cli_unknown_command(cmd);
    }
    else
    {
        _cli_commands_handlers[t](cmd + strlen(cmd) + 1);
    }
}