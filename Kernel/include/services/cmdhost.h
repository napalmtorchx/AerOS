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
KRESULT command_pci(int argc, char** argv);
KRESULT command_ls(int argc, char** argv);
KRESULT command_threads(int argc, char** argv);
KRESULT command_gui(int argc, char** argv);

static const command_t CMD_CLS      = (command_t){ "CLS",     "cls [fg:forecolor,bg:backcolor]", "Clear the screen", command_cls };
static const command_t CMD_HELP     = (command_t){ "HELP",    "help [u:usage,s:short,n:cmdname]", "Show list of commands", command_help };
static const command_t CMD_PCI      = (command_t){ "PCI",     "pci", "Show list of PCI devices", command_pci };
static const command_t CMD_LS       = (command_t){ "LS",      "ls [path]", "Show files in directory", command_ls };
static const command_t CMD_THREADS  = (command_t){ "THREADS", "threads", "Show list of running threads", command_threads };
static const command_t CMD_GUI      = (command_t){ "GUI",     "gui", "start graphical subsystem interface", command_gui };

static const command_t* COMMANDS[] = 
{
    &CMD_CLS,
    &CMD_HELP,
    &CMD_PCI,
    &CMD_LS,
    &CMD_THREADS,
    &CMD_GUI,
};

void cmdhost_init();
KRESULT cmdhost_main(int argc, char** argv);
void cmdhost_prompt();
void cmdhost_push(const char* input);
void cmdhost_toggle(bool enabled);
char* cmdhost_getpath();