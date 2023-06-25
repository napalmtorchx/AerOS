#include <core/cmdhost.h>
#include <kernel.h>

static thread_t* _thread;
static char**    _inqueue;
static size_t    _qsz;
static int       _count;

void cmdhost_init()
{
    _thread  = thread_create("cmdhost", 256 * KILOBYTE, cmdhost_main, THREAD_PRIORITY_NORMAL, 0, NULL);
    _qsz     = 4096;
    _count   = 0;
    _inqueue = (char**)malloc(sizeof(char*) * _qsz);
    memset(_inqueue, 0, sizeof(char*) * _qsz);

    taskmgr_load(_thread);
    taskmgr_start(_thread);

    debug_log("%s Initialized command host\n", DEBUG_OK);
}

KRESULT cmdhost_main(int argc, char** argv)
{
    while (true)
    {
        lock();
        
        int pos = 0;
        while (pos < _count)
        {
            char* input = (char*)_inqueue[pos];
            pos++;

            size_t cmd_argc = 0;
            char** cmd_argv = strsplit(input, ' ', &cmd_argc);
            if (cmd_argv == NULL) { continue; }
            if (cmd_argc == 0 && cmd_argv != NULL) { free(cmd_argv); continue; }

            char cmd[48];
            memset(cmd, 0, sizeof(cmd));
            strcpy(cmd, cmd_argv[0]);
            strupper(cmd);

            bool valid = false;
            static const size_t cmd_count = sizeof(COMMANDS) / sizeof(command_t*);
            for (size_t i = 0; i < cmd_count; i++)
            {
                if (COMMANDS[i]->handler != NULL && !strcmp(COMMANDS[i]->name, cmd))
                {
                    COMMANDS[i]->handler(cmd_argc, cmd_argv);
                    valid = true;
                    debug_log("%s Executed command - Name:%s Handler:%p\n", DEBUG_INFO, COMMANDS[i]->name, COMMANDS[i]->handler);
                    break;
                }
            }

            if (!valid) { debug_log("%s Invalid command '%s'\n", DEBUG_ERROR, cmd); }
            for (size_t i = 0; i < cmd_argc; i++) { free(cmd_argv[i]); }
            free(cmd_argv);
        }

        for (size_t i = 0; i < _count; i++) { free(_inqueue[i]); }
        _count = 0;

        taskmgr_schedule(true);
    }

    return KRESULT_SUCCESS;
}

void cmdhost_push(const char* input)
{
    if (input == NULL) { debug_error("cmdhost_push(%s) - Attempt to push null pointer to command queue", input); return; }

    bool locked = THIS_THREAD->context.lock;
    lock();
    char* input_dup = strdup(input);
    _inqueue[_count] = input_dup;
    _count++;

    if (!locked) { unlock(); }
}

// ------------------------------------------------------------------------------------------------------------------------------------------

KRESULT command_cls(int argc, char** argv)
{
    console_clear(kconsole_get());
    return KRESULT_SUCCESS;
}

KRESULT command_help(int argc, char** argv)
{
    console_t* term = kconsole_get();

    console_printf(term, "%s Showing list of commands:\n", DEBUG_INFO);

    static const size_t cmd_count = sizeof(COMMANDS) / sizeof(command_t*);
    for (size_t i = 0; i < cmd_count; i++)
    {
        if (COMMANDS[i]->handler == NULL) { continue; }
        console_write(term, COMMANDS[i]->name);
        console_setpos(term, (point_t){ 24, term->cursor.y });
        console_writeln(term, COMMANDS[i]->help);
    }

    return KRESULT_SUCCESS;
}