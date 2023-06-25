#pragma once
#include <lib/types.h>
#include <lib/ptrlist.h>
#include <lib/kresult.h>

// Function handler for commands
typedef KRESULT (*command_handler_t)(int argc, char** argv);

// Structure for managing command information
typedef struct
{
    char              name[48];
    char              usage[128];
    char              help[128];
    command_handler_t handler;
} command_t;

KRESULT command_cls(int argc, char** argv);
KRESULT command_help(int argc, char** argv);

static const command_t CMD_CLS  = (command_t){ "CLS",  "cls [fg:forecolor,bg:backcolor]", "Clear the screen", command_cls };
static const command_t CMD_HELP = (command_t){ "HELP", "help [u:usage,s:short,n:cmdname]", "Show list of commands", command_help };

static const command_t* COMMANDS[] = 
{
    &CMD_CLS,
    &CMD_HELP,
};

void cmdhost_init();
KRESULT cmdhost_main(int argc, char** argv);
void cmdhost_push(const char* input);