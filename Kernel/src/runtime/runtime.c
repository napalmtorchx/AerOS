#include <runtime/runtime.h>
#include <kernel.h>

runtime_t runtime_create(uint8_t* program, size_t progsz, size_t stacksz)
{
    if (program == NULL) { debug_log("%s Creating VM with no executable\n", DEBUG_WARN); }

    runtime_t runtime;
    runtime.program    = program;
    runtime.programsz  = progsz;
    runtime.exec       = (executable_t*)runtime.program;
    runtime.stacksz    = memalign(stacksz, 4);
    runtime.stack      = (uint32_t*)malloc(runtime.stacksz);
    runtime.mmap       = NULL;
    runtime.mmap_count = 0;
    memset(&runtime.regs.raw, 0, sizeof(runtime.regs.raw));
    runtime_map(&runtime, runtime.program, runtime.programsz, false);
    runtime_map(&runtime, runtime.stack, runtime.stacksz, false);

    runtime.regs.vals.bp = (uint32_t)runtime.stack;
    runtime.regs.vals.sp = (uint32_t)runtime.stack;
    runtime.regs.vals.ip = (uint32_t)runtime.program + sizeof(executable_t) + runtime.exec->text_offset;

    //valid
    runtime_wrl(&runtime, (uint32_t)runtime.stack, 0xDEADC0DE);
    
    debug_log("%s Created runtime - Exec:%s(%2x) Stack:%p-%p\n",
                DEBUG_INFO, runtime.exec == NULL ? "NULL" : runtime.exec->name, runtime.exec == NULL ? 0 : runtime.exec->type, runtime.stack, (uintptr_t)runtime.stack + runtime.stacksz);

    return runtime;
}

void runtime_step(runtime_t* runtime)
{
    if (runtime == NULL) { debug_error("runtime_step(%p) - Null runtime pointer", runtime); return; }
}

memblk_t* runtime_map(runtime_t* runtime, uint32_t addr, size_t sz, bool readonly)
{
    if (runtime == NULL) { debug_error("runtime_map(%p, %p, %a, %d) - Null runtime pointer", runtime, addr, sz, readonly); return NULL; }
    if (runtime_ismapped(runtime, addr)) { debug_error("runtime_map(%p, %p, %a, %d) - Address is already mapped", runtime, addr, sz, readonly); return NULL; }

    memblk_t* mmap = (memblk_t*)malloc(sizeof(memblk_t) * (runtime->mmap_count + 1));
    if (runtime->mmap != NULL) { memcpy(mmap, runtime->mmap, sizeof(memblk_t) * runtime->mmap_count); free(runtime->mmap); }

    runtime->mmap = mmap;
    runtime->mmap[runtime->mmap_count] = (memblk_t){ addr, sz, readonly ? MEM_VM_READONLY : MEM_VM_RW };
    runtime->mmap_count++;

    debug_log("%s Runtime(%p) map - Addr:%p-%p Size:%a RW:%d\n", DEBUG_INFO, runtime, addr, addr + sz, sz, !readonly);
    return &runtime->mmap[runtime->mmap_count - 1];
}

void runtime_munmap(runtime_t* runtime, uint32_t addr)
{
    if (runtime == NULL) { debug_error("runtime_map(%p, %p) - Null runtime pointer", runtime, addr); return; }
}

bool runtime_ismapped(runtime_t* runtime, uint32_t addr)
{
    if (runtime == NULL) { return false; }
    for (size_t i = 0; i < runtime->mmap_count; i++)
    {
        if (runtime->mmap[i].sz == 0)      { continue; }
        if (runtime->mmap[i].addr == addr) { return true; }
    }
    return false;
}

void runtime_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr)
{
    if (runtime == NULL) { debug_error("runtime_checkaddr(%p, %p, %a, %d) - Null runtime pointer", runtime, addr, sz, wr); return; }
    KRESULT res = runtime_try_checkaddr(runtime, addr, sz, wr);
    if (res == KRESULT_SUCCESS) { return; }
    if (res == KRESULT_VM_WRVIOLATION) { debug_error("runtime_checkaddr(%p, %p, %a, %d) - Write violation", runtime, addr, sz, wr); return; }
    if (res == KRESULT_VM_RDVIOLATION) { debug_error("runtime_checkaddr(%p, %p, %a, %d) - Read violation",  runtime, addr, sz, wr); return; }
}

KRESULT runtime_try_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr)
{
    if (runtime == NULL) { return KRESULT_NULL_PTR; }
    for (size_t i = 0; i < runtime->mmap_count; i++)
    {
        if (addr >= runtime->mmap[i].addr && addr + sz < runtime->mmap[i].addr + runtime->mmap[i].sz)
        {
            if (runtime->mmap[i].type == MEM_VM_READONLY && wr) { return KRESULT_VM_WRVIOLATION; }
            return KRESULT_SUCCESS;
        }
    }
    return (wr ? KRESULT_VM_WRVIOLATION : KRESULT_VM_RDVIOLATION);
}

uint8_t runtime_nextb(runtime_t* runtime)
{

}

uint16_t runtime_nextw(runtime_t* runtime)
{

}

uint32_t runtime_nextl(runtime_t* runtime)
{

}

uint8_t runtime_rdb(runtime_t* runtime, uint32_t addr)
{
    runtime_checkaddr(runtime, addr, 1, false);
    return *((uint8_t*)addr);
}

uint16_t runtime_rdw(runtime_t* runtime, uint32_t addr)
{
    runtime_checkaddr(runtime, addr, 2, false);
    return *((uint16_t*)addr);
}

uint32_t runtime_rdl(runtime_t* runtime, uint32_t addr)
{
    runtime_checkaddr(runtime, addr, 4, false);
    return *((uint32_t*)addr);
}

void runtime_wrb(runtime_t* runtime, uint32_t addr, uint8_t value)
{
    runtime_checkaddr(runtime, addr, 1, true);
    *((uint8_t*)addr) = value;
}

void runtime_wrw(runtime_t* runtime, uint32_t addr, uint16_t value)
{
    runtime_checkaddr(runtime, addr, 2, true);
    *((uint16_t*)addr) = value;
}

void runtime_wrl(runtime_t* runtime, uint32_t addr, uint32_t value)
{
    runtime_checkaddr(runtime, addr, 4, true);
    *((uint32_t*)addr) = value;
}