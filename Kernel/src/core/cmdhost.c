#include <core/cmdhost.h>
#include <kernel.h>

static thread_t* _thread;

void cmdhost_init()
{
    _thread = thread_create("cmdhost", 256 * KILOBYTE, cmdhost_main, THREAD_PRIORITY_NORMAL, 0, NULL);

    taskmgr_load(_thread);
    taskmgr_start(_thread);

    debug_log("%s Initialized command host\n", DEBUG_OK);
}

KRESULT cmdhost_main(int argc, char** argv)
{
    while (true)
    {
        lock();
        taskmgr_schedule(true);
    }

    return KRESULT_SUCCESS;
}

void cmdhost_push(const char* input)
{
    if (input == NULL) { debug_error("cmdhost_push(%s) - Attempt to push null pointer to command queue", input); return; }
}

// ------------------------------------------------------------------------------------------------------------------------------------------

KRESULT command_cls(int argc, char** argv)
{
    return KRESULT_SUCCESS;
}

KRESULT command_help(int argc, char** argv)
{
    return KRESULT_SUCCESS;
}