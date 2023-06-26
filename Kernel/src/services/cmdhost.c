#include <services/cmdhost.h>
#include <kernel.h>

static thread_t*   _thread;
static char**      _inqueue;
static size_t      _qsz;
static int         _count;
static kbd_state_t _kb;
static char        _kbuff[0x10000];
static char        _path[FILENAME_MAX];
static bool        _sending;
static bool        _processing;
static bool        _enabled;

void cmdhost_on_char(void* handle, char c)
{
    if (!_enabled) { return; }
    console_printf(kconsole_get(), "%c", c);
}

void cmdhost_on_del(void* handle, void* unused)
{
    if (!_enabled || strlen(_kbuff) == 0) { return; }
    console_delete(kconsole_get(), 1);
}

void cmdhost_on_ret(void* handle, void* unused) { }

void cmdhost_init()
{
    _thread     = thread_create("cmdhost", 256 * KILOBYTE, cmdhost_main, THREAD_PRIORITY_NORMAL, 0, NULL);
    _qsz        = 4096;
    _count      = 0;
    _sending    = false;
    _processing = false;
    
    _inqueue = (char**)malloc(sizeof(char*) * _qsz);
    memset(_inqueue, 0, sizeof(char*) * _qsz);

    memset(_path, 0, sizeof(_path));
    strcpy(_path, "A:/");

    memset(_kbuff, 0, sizeof(_kbuff));
    _kb = kbd_state_create(_kbuff, sizeof(_kbuff), NULL, false, cmdhost_on_char, cmdhost_on_del, cmdhost_on_ret);

    taskmgr_load(_thread);
    taskmgr_start(_thread);

    _enabled = true;
    debug_log("%s Initialized command host\n", DEBUG_OK);
    console_clear(kconsole_get());
    console_printf(kconsole_get(), "AerOS version 2.0\nRAM Usage:%u/%u KB\n", heap_get_used_mem(kernel_heap_ref()) / KILOBYTE, heap_get_total_mem(kernel_heap_ref()) / KILOBYTE);
}

void cmdhost_prompt()
{
    console_printf(kconsole_get(), "%s%s%s> ", ANSI_FG_CYAN, _path, ANSI_RESET);
}

KRESULT cmdhost_main(int argc, char** argv)
{
    lock();
    cmdhost_prompt();
    unlock();

    while (true)
    {
        lock();

        if (_enabled)
        {
            kbd_setstate(&_kb);
            if (!_processing)
            {
                if (kbd_keydown(KEY_ENTER) && !_sending)
                {
                    _processing = true;
                    _sending = true;
                    debug_log("%s Command '%s' has been queued\n", DEBUG_INFO, _kbuff);
                    console_newline(kconsole_get());
                    cmdhost_push(_kbuff);
                    memset(_kbuff, 0, sizeof(_kbuff));
                }   
                if (kbd_keyup(KEY_ENTER)) { _sending = false; }
            }
        }
        
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

        if (_processing)
        {
            cmdhost_prompt();
            _processing = false;
        }

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

void cmdhost_toggle(bool enabled) { _enabled = enabled; }

char* cmdhost_getpath() { return _path; }

// ------------------------------------------------------------------------------------------------------------------------------------------

KRESULT command_cls(int argc, char** argv)
{
    debug_log("DOING CLS\n");
    console_clear(kconsole_get());
    return KRESULT_SUCCESS;
}

KRESULT command_pci(int argc, char** argv)
{
    pci_list_devices();
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

KRESULT command_ls(int argc, char** argv)
{
    console_t* term = kconsole_get();

    char path[FILENAME_MAX];
    memset(path, 0, sizeof(path));

    if (argc < 2) { strcat(path, _path); } else { for (size_t i = 1; i < argc; i++) { strcat(path, argv[i]); } }
    if (!virtfs_dexists(path)) { console_writeln(term, "No files found."); return KRESULT_SUCCESS; }
    
    ptrlist_t dirs  = virtfs_getdirs(path);
    ptrlist_t files = virtfs_getfiles(path);
    if (dirs.count == 0 && files.count == 0) { console_writeln(term, "No files found"); ptrlist_free(&dirs, true); ptrlist_free(&files, true); return KRESULT_SUCCESS; }

    for (size_t i = 0; i < dirs.count; i++) 
    { 
        console_printf(term, "- %s%s%s\n", ANSI_FG_YELLOW, (char*)dirs.entries[i], ANSI_RESET); 
        free(dirs.entries[i]);
    }

    for (size_t i = 0; i < files.count; i += 2) 
    { 
        char*  file = (char*)files.entries[i];
        size_t sz   = (size_t)files.entries[i + 1];

        console_write(term, file); 
        console_setpos(term, (point_t){ 24, term->cursor.y });
        console_printf(term, "%a\n", sz);
        free(file);
    }

    free(dirs.entries);
    free(files.entries);
    return KRESULT_SUCCESS;
}

KRESULT command_threads(int argc, char** argv)
{
    taskmgr_print(true);
    return KRESULT_SUCCESS;
}

KRESULT command_gui(int argc, char** argv)
{
    gfx_init();
    _enabled = false;
    return KRESULT_SUCCESS;
}