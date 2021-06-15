#include "cli.h"

#define MAX_CLI_COMMAND_SIZE        (10)
#define INDEX_UNKNOWN               (-1)

typedef void(*cli_callback)(char *);

static void _cli_help_command(char *cmd);
extern void cli_sha1_handler(char *cmd);
extern void cli_sha256_handler(char *cmd);
extern void cli_sha512_handler(char *cmd);
extern void cli_pin_handler(char *cmd);
extern void cli_pbkdf2_handler(char *cmd);

static const char _cli_commands[][MAX_CLI_COMMAND_SIZE] = 
{
    "help",
    "sha1",
    "sha256",
    "sha512",
    "pin",
    "pbkdf2",
    "reboot",
    "thread",
    "load",
    "dfu",
    "memshow",
    "cd",
    "panic",
    "sysdown",
    "memremap",
    "flash",
    "serial",
    "check",
    "selftest",
    "dma",
};

static const cli_callback _cli_commands_handlers[] = 
{
    _cli_help_command,
    cli_sha1_handler,
    cli_sha256_handler,
    cli_sha512_handler,
    cli_pin_handler,
    cli_pbkdf2_handler,
};

static int32_t _cli_find_cmd_index(char *cmd)
{
    char *ptr = strtok(cmd, " ");
    int32_t ret = INDEX_UNKNOWN;

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

static void _cli_help_command(char *cmd)
{
    UNUSED(cmd);
    printf("List of supported commands!\n\r");
    printf("For more information type: '<command> -h'!\n\r");

    for (uint32_t i = 0; i < sizeof(_cli_commands) / MAX_CLI_COMMAND_SIZE; ++i)
    {
        printf("    %s\n\r", _cli_commands[i]);
    }
}


void cli_handler(char *cmd)
{
    uint32_t t = _cli_find_cmd_index(cmd);

    if (t == INDEX_UNKNOWN)
    {
        _cli_unknown_command(cmd);
    }
    else
    {
        _cli_commands_handlers[t](cmd + strlen(cmd) + 1);
    }
}