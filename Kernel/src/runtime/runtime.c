#include <runtime/runtime.h>
#include <kernel.h>

runtime_t runtime_create(uint8_t* program, size_t progsz, size_t stacksz)
{
    // validate program data
    if (program == NULL) { debug_log("%s Creating VM with no executable\n", DEBUG_WARN); }

    // create virtual machine
    runtime_t runtime;
    runtime.running = false;

    // set raw program data properties
    runtime.program   = program;
    runtime.programsz = (program == NULL ? 0 : progsz);
    runtime.exec      = (executable_t*)(program == NULL ? NULL : runtime.program);

    // create virtual machine stack
    runtime.stacksz = memalign(stacksz, 4);
    runtime.stack   = (uint32_t*)malloc(runtime.stacksz);

    // initialize memory map
    runtime.mmap       = NULL;
    runtime.mmap_count = 0;
    runtime_map(&runtime, runtime.program, runtime.programsz, false);
    runtime_map(&runtime, runtime.stack, runtime.stacksz, false);

    // initialize registers
    memset(&runtime.regs.raw, 0, sizeof(runtime.regs.raw));
    runtime.regs.vals.bp = (uint32_t)runtime.stack;
    runtime.regs.vals.sp = (uint32_t)runtime.stack;
    runtime.regs.vals.ip = (uint32_t)runtime.program + sizeof(executable_t) + runtime.exec->text_offset;

    // log information about creation of virtual machine
    debug_log("%s Created runtime - Exec:%s(%2x) Stack:%p-%p Text:%p(%u bytes) Data:%p(%u bytes)\n",
                DEBUG_INFO, runtime.exec == NULL ? "NULL" : runtime.exec->name, runtime.exec == NULL ? 0 : runtime.exec->type, runtime.stack, (uintptr_t)runtime.stack + runtime.stacksz,
                runtime.exec->text_offset, runtime.exec->text_sz, runtime.exec->data_offset, runtime.exec->data_sz);

    // return virtual machine instance
    return runtime;
}

void runtime_run(runtime_t* runtime)
{
    // set running flag before entering loop, will run until exit has been called or error has occurred
    runtime->running = true;

    // main loop
    while (true)
    {
        // execute single instruction
        runtime_step(runtime);

        // check if virtual machine has stopped running, if so exit loop
        if (!runtime->running) { break; }
    }

    // log information about machine closure
    debug_log("%s Runtime instance finished\n", DEBUG_INFO);
}

void runtime_setflags(runtime_t* runtime, uint64_t result)
{
    // check if value is greater than what a register is capable of containing
    runtime->regs.vals.flags.vals.carry = (result > UINT32_MAX);

    // check if value is equal to zero
    runtime->regs.vals.flags.vals.zero  = (result == 0);
}

void runtime_step(runtime_t* runtime)
{
    // validate virtual machine instance
    if (runtime == NULL) { debug_error("runtime_step(%p) - Null runtime pointer", runtime); return; }

    // fetch next opcode from memory at IP
    uint8_t op = runtime_nextb(runtime);

    // check opcode value and execute operation occordingly
    switch (op)
    {
        default:            { debug_error("runtime_step(%p) - Invalid opcode %2x", runtime, op); return; }
        case RTOP_NOP:      { break; }
        case RTOP_EXIT:     { runtime->running = false;           break; }
        case RTOP_ADD:      { runtime_arith(runtime, false, '+'); break; }
        case RTOP_ADDR:     { runtime_arith(runtime, true,  '+'); break; }
        case RTOP_SUB:      { runtime_arith(runtime, false, '-'); break; }
        case RTOP_SUBR:     { runtime_arith(runtime, true,  '-'); break; }
        case RTOP_MUL:      { runtime_arith(runtime, false, '*'); break; }
        case RTOP_MULR:     { runtime_arith(runtime, true,  '*'); break; }
        case RTOP_DIV:      { runtime_arith(runtime, false, '/'); break; }
        case RTOP_DIVR:     { runtime_arith(runtime, true,  '/'); break; }
        case RTOP_OR:       { runtime_arith(runtime, false, '|'); break; }
        case RTOP_ORR:      { runtime_arith(runtime, true,  '|'); break; }
        case RTOP_XOR:      { runtime_arith(runtime, false, '^'); break; }
        case RTOP_XORR:     { runtime_arith(runtime, true,  '^'); break; }
        case RTOP_AND:      { runtime_arith(runtime, false, '&'); break; }
        case RTOP_ANDR:     { runtime_arith(runtime, true,  '&'); break; }
        case RTOP_NOT:      { runtime_arith(runtime, false, '~'); break; }
        case RTOP_SHL:      { runtime_arith(runtime, false, '<'); break; }
        case RTOP_SHLR:     { runtime_arith(runtime, true,  '<'); break; }
        case RTOP_SHR:      { runtime_arith(runtime, false, '>'); break; }
        case RTOP_SHRR:     { runtime_arith(runtime, true,  '>'); break; }
        case RTOP_JMP:      { runtime_jump(runtime,  false);      break; }
        case RTOP_JMPR:     { runtime_jump(runtime,  true);       break; }
        case RTOP_CALL:     { runtime_call(runtime,  false);      break; }
        case RTOP_CALLR:    { runtime_call(runtime,  true);       break; }
        case RTOP_RET:      { runtime_pop(runtime,   0xFF);       break; }
        case RTOP_PUSH:     { runtime_push(runtime,  false);      break; }
        case RTOP_PUSHR:    { runtime_push(runtime,  true);       break; }
        case RTOP_POP:      { runtime_pop(runtime,   true);       break; }
        case RTOP_SE:       { runtime_skip(runtime,  false, '='); break; }
        case RTOP_SER:      { runtime_skip(runtime,  true,  '='); break; }
        case RTOP_SNE:      { runtime_skip(runtime,  false, '!'); break; }
        case RTOP_SNER:     { runtime_skip(runtime,  true,  '!'); break; }
        case RTOP_SGT:      { runtime_skip(runtime,  false, '>'); break; }
        case RTOP_SGTR:     { runtime_skip(runtime,  true,  '>'); break; }
        case RTOP_SLT:      { runtime_skip(runtime,  false, '<'); break; }
        case RTOP_SLTR:     { runtime_skip(runtime,  true,  '<'); break; }
        case RTOP_SC:       { runtime_skip(runtime,  false, 'c'); break; }
        case RTOP_SN:       { runtime_skip(runtime,  false, 'n'); break; }
        case RTOP_SZ:       { runtime_skip(runtime,  false, 'z'); break; }
        case RTOP_LD:       { runtime_ldreg(runtime, 'i');        break; }
        case RTOP_LDR:      { runtime_ldreg(runtime, 'r');        break; }
        case RTOP_LDB:      { runtime_ldreg(runtime, 'b');        break; }
        case RTOP_LDW:      { runtime_ldreg(runtime, 'w');        break; }
        case RTOP_LDL:      { runtime_ldreg(runtime, 'l');        break; }
        case RTOP_LDRB:     { runtime_ldreg(runtime, 'B');        break; }
        case RTOP_LDRW:     { runtime_ldreg(runtime, 'W');        break; }
        case RTOP_LDRL:     { runtime_ldreg(runtime, 'L');        break; }
        case RTOP_WRB:      { runtime_wrreg(runtime, 'b');        break; }
        case RTOP_WRW:      { runtime_wrreg(runtime, 'w');        break; }
        case RTOP_WRL:      { runtime_wrreg(runtime, 'l');        break; }
        case RTOP_WRRB:     { runtime_wrreg(runtime, 'B');        break; }
        case RTOP_WRRW:     { runtime_wrreg(runtime, 'W');        break; }
        case RTOP_WRRL:     { runtime_wrreg(runtime, 'L');        break; }
        case RTOP_STB:      { runtime_streg(runtime, 'b');        break; }
        case RTOP_STW:      { runtime_streg(runtime, 'w');        break; }
        case RTOP_STL:      { runtime_streg(runtime, 'l');        break; }
        case RTOP_STRB:     { runtime_streg(runtime, 'B');        break; }
        case RTOP_STRW:     { runtime_streg(runtime, 'W');        break; }
        case RTOP_STRL:     { runtime_streg(runtime, 'L');        break; }
        case RTOP_SYSCALL:  { runtime_syscall(runtime, false);    break; }
        case RTOP_SYSCALLR: { runtime_syscall(runtime, true);     break; }
    }

    // log information about the status of the virtual machine after executing the instruction
    debug_log("%s Runtime executed instruction %2x\n", DEBUG_INFO, op);
    debug_log("         R0:%p R1:%p R2:%p R3:%p\n", runtime->regs.vals.r0, runtime->regs.vals.r1, runtime->regs.vals.r2, runtime->regs.vals.r3);
    debug_log("         R4:%p R5:%p R6:%p R7:%p\n", runtime->regs.vals.r4, runtime->regs.vals.r5, runtime->regs.vals.r6, runtime->regs.vals.r7);
    debug_log("         SP:%p BP:%p IP:%p FL:%p\n", runtime->regs.vals.sp, runtime->regs.vals.bp, runtime->regs.vals.ip, runtime->regs.vals.flags.raw);
}

memblk_t* runtime_map(runtime_t* runtime, uint32_t addr, size_t sz, bool readonly)
{
    // validate runtime and requested region to map
    if (runtime == NULL) { debug_error("runtime_map(%p, %p, %a, %d) - Null runtime pointer", runtime, addr, sz, readonly); return NULL; }
    if (runtime_ismapped(runtime, addr)) { debug_error("runtime_map(%p, %p, %a, %d) - Address is already mapped", runtime, addr, sz, readonly); return NULL; }

    // create larger copy of mmap entries
    memblk_t* mmap = (memblk_t*)malloc(sizeof(memblk_t) * (runtime->mmap_count + 1));
    if (runtime->mmap != NULL) { memcpy(mmap, runtime->mmap, sizeof(memblk_t) * runtime->mmap_count); free(runtime->mmap); }

    // replace old copy of mmap entries
    runtime->mmap = mmap;
    // set the last entry in list to the requested region
    runtime->mmap[runtime->mmap_count] = (memblk_t){ addr, sz, readonly ? MEM_VM_READONLY : MEM_VM_RW };
    // increment list count
    runtime->mmap_count++;

    // log information about memory map and return pointer to mmap entry
    debug_log("%s Runtime(%p) map - Addr:%p-%p Size:%a RW:%d\n", DEBUG_INFO, runtime, addr, addr + sz, sz, !readonly);
    return &runtime->mmap[runtime->mmap_count - 1];
}

void runtime_munmap(runtime_t* runtime, uint32_t addr)
{
    // validate runtime and memory region
    if (runtime == NULL) { debug_error("runtime_map(%p, %p) - Null runtime pointer", runtime, addr); return; }
    if (!runtime_ismapped(runtime, addr)) { debug_error("runtime_munmap(%p, %p) - Address is not mapped", runtime, addr); return; }
    
    // get index of mmap entry based on address
    int index = -1;
    for (size_t i = 0; i < runtime->mmap_count; i++)
    {
        // located entry - set index and end loop
        if (runtime->mmap[i].addr == addr) { index = (int)i; break; }
    }

    // index is still invalid - was not able to locate in mmap
    if (index == -1) { debug_error("runtime_munmap(%p, %p) - Unable to match address to entry in mmap", runtime, addr); return; }    

    // get the memory block at specified address
    memblk_t blk = runtime->mmap[index];

    // if it is the last/only entry in list, no need to allocate more memory, delete mmap entry and list
    if (index == 0 && runtime->mmap_count == 1) 
    { 
        free(runtime->mmap); 
        runtime->mmap       = NULL;
        runtime->mmap_count = 0;
    }
    // there is more than one entry, allocate smaller list to store mmap entries
    else
    {
        // allocate memory for mmap entries
        memblk_t* mmap = (memblk_t*)malloc(sizeof(memblk_t) * (runtime->mmap_count - 1));

        // copy all entries except the one at the index located earlier
        size_t i, j = 0;
        for (i = 0; i < runtime->mmap_count; i++)
        {
            if (i != index) { mmap[j] = runtime->mmap[i]; j++; }
        }

        // replace the old mmap list
        free(runtime->mmap);
        runtime->mmap = mmap;
        // decrement list count
        runtime->mmap_count--;
    }

    // log information about memory unmap
    debug_log("%s Runtime(%p) unmap - Addr:%p-%p Size:%a RW:%d\n", DEBUG_INFO, runtime, blk.addr, blk.addr + blk.sz, blk.sz, (blk.type == MEM_VM_READONLY ? 1 : 0));
}

bool runtime_ismapped(runtime_t* runtime, uint32_t addr)
{
    // obviously is runtime is null, we can't validate this address
    if (runtime == NULL) { return false; }

    // iterate through all memory map entries and check if one exists at the specified starting address
    for (size_t i = 0; i < runtime->mmap_count; i++)
    {
        if (runtime->mmap[i].sz == 0)      { continue; }
        if (runtime->mmap[i].addr == addr) { return true; }
    }

    // unable to locate memory map entry in list
    return false;
}

void runtime_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr)
{
    // validate runtime instance
    if (runtime == NULL) { debug_error("runtime_checkaddr(%p, %p, %a, %d) - Null runtime pointer", runtime, addr, sz, wr); return; }

    // check if specified memory region is valid
    KRESULT res = runtime_try_checkaddr(runtime, addr, sz, wr);

    // validate results
    if (res == KRESULT_SUCCESS) { return; }
    if (res == KRESULT_VM_WRVIOLATION) { debug_error("runtime_checkaddr(%p, %p, %a, %d) - Write violation", runtime, addr, sz, wr); return; }
    if (res == KRESULT_VM_RDVIOLATION) { debug_error("runtime_checkaddr(%p, %p, %a, %d) - Read violation",  runtime, addr, sz, wr); return; }
}

KRESULT runtime_try_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr)
{
    // validate runtime instance
    if (runtime == NULL) { return KRESULT_NULL_PTR; }

    // iterate through memory map
    for (size_t i = 0; i < runtime->mmap_count; i++)
    {
        // if specified address and size is within bounds of 1 or more memory maps
        if (addr >= runtime->mmap[i].addr && addr + sz <= runtime->mmap[i].addr + runtime->mmap[i].sz)
        {
            // make sure read/write flag is valid
            if (runtime->mmap[i].type == MEM_VM_READONLY && wr) { return KRESULT_VM_WRVIOLATION; }

            // read/write condition has been met, return success
            return KRESULT_SUCCESS;
        }
    }

    // falied match with any mmap entries - cannot validate
    return (wr ? KRESULT_VM_WRVIOLATION : KRESULT_VM_RDVIOLATION);
}

uint8_t runtime_nextb(runtime_t* runtime)
{
    uint8_t v = runtime_rdb(runtime, runtime->regs.vals.ip);
    runtime->regs.vals.ip++;
    return v;
}

uint16_t runtime_nextw(runtime_t* runtime)
{
    uint16_t v = runtime_rdw(runtime, runtime->regs.vals.ip);
    runtime->regs.vals.ip += 2;
    return v;
}

uint32_t runtime_nextl(runtime_t* runtime)
{
    uint32_t v = runtime_rdl(runtime, runtime->regs.vals.ip);
    runtime->regs.vals.ip += 4;
    return v;
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