#include <runtime/isa.h>
#include <kernel.h>

void runtime_arith_imm(runtime_t* runtime, char type)
{
    uint64_t result = 0;
    uint32_t value  = 0;
    uint8_t  reg    = runtime_nextb(runtime);

    if (type == '<' || type == '>') { value = runtime_nextb(runtime); }
    else { value = runtime_nextl(runtime); }

    switch (type)
    {
        case '+': { result = runtime->regs.raw[reg] +  value; break; }
        case '-': { result = runtime->regs.raw[reg] -  value; break; }
        case '*': { result = runtime->regs.raw[reg] *  value; break; }
        case '/': { result = runtime->regs.raw[reg] /  value; break; }
        case '|': { result = runtime->regs.raw[reg] |  value; break; }
        case '^': { result = runtime->regs.raw[reg] ^  value; break; }
        case '&': { result = runtime->regs.raw[reg] &  value; break; }
        case '<': { result = runtime->regs.raw[reg] << value; break; }
        case '>': { result = runtime->regs.raw[reg] >> value; break; }
    }
    runtime->regs.raw[reg] = result;
    runtime_setflags(runtime, result);
}

void runtime_arith_reg(runtime_t* runtime, char type)
{
    uint64_t result = 0;
    uint8_t  rx     = runtime_nextb(runtime);
    uint8_t  ry     = runtime_nextb(runtime);

    switch (type)
    {
        case '+': { result = runtime->regs.raw[rx] +  runtime->regs.raw[ry]; break; }
        case '-': { result = runtime->regs.raw[rx] -  runtime->regs.raw[ry]; break; }
        case '*': { result = runtime->regs.raw[rx] *  runtime->regs.raw[ry]; break; }
        case '/': { result = runtime->regs.raw[rx] /  runtime->regs.raw[ry]; break; }
        case '|': { result = runtime->regs.raw[rx] |  runtime->regs.raw[ry]; break; }
        case '^': { result = runtime->regs.raw[rx] ^  runtime->regs.raw[ry]; break; }
        case '&': { result = runtime->regs.raw[rx] &  runtime->regs.raw[ry]; break; }
        case '<': { result = runtime->regs.raw[rx] << runtime->regs.raw[ry]; break; }
        case '>': { result = runtime->regs.raw[rx] >> runtime->regs.raw[ry]; break; }
    }
    runtime->regs.raw[rx] = result;
    runtime_setflags(runtime, result);
}

void runtime_arith(runtime_t* runtime, bool is_regval, char type)
{
    bool valid = false;

    if (type == '+' || type == '-' || type == '*' || type == '/' || type == '|' || type == '^' || type == '&' || type == '<' || type == '>')
    {
        valid = true;
        if (is_regval) { runtime_arith_reg(runtime, type); }
        else           { runtime_arith_imm(runtime, type); }
        return;
    }
    else if (type == '~')
    {
        uint8_t reg = runtime_nextb(runtime);
        runtime->regs.raw[reg] = ~runtime->regs.raw[reg];
    }
    debug_error("runtime_arith(%p, %d, '%c') - Invalid operation '%c'", runtime, is_regval, type, type);
}

void runtime_jump(runtime_t* runtime, bool is_regval)
{
    uint32_t addr = 0;
    if (is_regval) { addr = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { addr = runtime_nextl(runtime); }
    runtime->regs.vals.ip = addr;
}

void runtime_call(runtime_t* runtime, bool is_regval)
{
    if (runtime->regs.vals.sp >= (uint32_t)runtime->stack + runtime->stacksz) { debug_error("runtime_call(%p, %d) - Stack overflow", runtime, is_regval); return; }

    uint32_t addr = 0;
    if (is_regval) { addr = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { addr = runtime_nextl(runtime); }
    
    uint8_t next_op = runtime_rdb(runtime, runtime->regs.vals.ip);
    runtime_wrl(runtime, runtime->regs.vals.sp, RUNTIME_OPCODE_BYTES[next_op]);
    runtime->regs.vals.sp += 4;
    runtime->regs.vals.ip = addr;
}

void runtime_pop(runtime_t* runtime, uint8_t reg)
{
    runtime->regs.vals.sp -= 4;
    if (runtime->regs.vals.sp < (uint32_t)runtime->stack) { debug_error("runtime_ret(%p, %2x) - Stack underflow", runtime, reg); return; }
    if (reg != 0xFF) { runtime->regs.raw[reg] = runtime_rdl(runtime, runtime->regs.vals.sp); }
}

void runtime_push(runtime_t* runtime, bool is_regval)
{
    if (runtime->regs.vals.sp >= (uint32_t)runtime->stack + runtime->stacksz) { debug_error("runtime_push(%p, %d) - Stack overflow", runtime, is_regval); return; }

    uint32_t val = 0;
    if (is_regval) { val = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { val = runtime_nextl(runtime); }

    runtime_wrl(runtime, runtime->regs.vals.sp, val);
    runtime->regs.vals.sp += 4;
}

void runtime_skip(runtime_t* runtime, bool is_regval, char type)
{
    uint8_t arg0 = runtime_nextb(runtime);


    uint32_t val = 0;
    if (type == 'c' || type == 'n' || type == 'z') { val = runtime_nextb(runtime); }
    else if (is_regval) { val = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { val = runtime_nextl(runtime); }

    uint8_t next_op = runtime_rdb(runtime, runtime->regs.vals.ip);

    bool cond = false;
    if (type == '=') { cond = (runtime->regs.raw[arg0] == val); }
    if (type == '!') { cond = (runtime->regs.raw[arg0] != val); }
    if (type == '<') { cond = (runtime->regs.raw[arg0] < val); }
    if (type == '>') { cond = (runtime->regs.raw[arg0] > val); }
    if (type == 'c') { cond = (runtime->regs.vals.flags.vals.carry    == arg0); }
    if (type == 'z') { cond = (runtime->regs.vals.flags.vals.zero     == arg0); }
    if (type == 'n') { cond = (runtime->regs.vals.flags.vals.negative == arg0); }
    if (cond) { runtime->regs.vals.ip += RUNTIME_OPCODE_BYTES[next_op]; }
}

void runtime_ldreg(runtime_t* runtime, char type)
{
    uint8_t reg = runtime_nextb(runtime);
    uint32_t value = 0;

    if (type == 'i') { value = runtime_nextl(runtime); }
    else if (type == 'r') { value = runtime->regs.raw[runtime_nextb(runtime)]; }
    else if (type == 'b') { value = runtime_rdb(runtime, runtime_nextl(runtime)); }
    else if (type == 'w') { value = runtime_rdw(runtime, runtime_nextl(runtime)); }
    else if (type == 'l') { value = runtime_rdl(runtime, runtime_nextl(runtime)); }
    else if (type == 'B') { value = runtime_rdb(runtime, runtime->regs.raw[runtime_nextb(runtime)]); }
    else if (type == 'W') { value = runtime_rdw(runtime, runtime->regs.raw[runtime_nextb(runtime)]); }
    else if (type == 'L') { value = runtime_rdl(runtime, runtime->regs.raw[runtime_nextb(runtime)]); }
    runtime->regs.raw[reg] = value;
}

void runtime_wrreg(runtime_t* runtime, char type)
{
    uint32_t addr = 0;
    uint32_t value = 0;

    if (type == 'b')
    {
        addr  = runtime_nextl(runtime);
        value = runtime_nextb(runtime);
        runtime_wrb(runtime, addr, (uint8_t)value);
    }
    else if (type == 'w')
    {
        addr  = runtime_nextl(runtime);
        value = runtime_nextw(runtime);
        runtime_wrw(runtime, addr, (uint16_t)value);
    }
    else if (type == 'l')
    {
        addr  = runtime_nextl(runtime);
        value = runtime_nextl(runtime);
        runtime_wrw(runtime, addr, value);
    }
    else if (type == 'B')
    {
        addr  = runtime->regs.raw[runtime_nextb(runtime)];
        value = runtime_nextb(runtime);
        runtime_wrb(runtime, addr, (uint8_t)value);
    }
    else if (type == 'W')
    {
        addr  = runtime->regs.raw[runtime_nextb(runtime)];
        value = runtime_nextw(runtime);
        runtime_wrw(runtime, addr, (uint16_t)value);
    }
    else if (type == 'L')
    {
        addr  = runtime->regs.raw[runtime_nextb(runtime)];
        value = runtime_nextl(runtime);
        runtime_wrw(runtime, addr, value);
    }
}


void runtime_streg(runtime_t* runtime, char type)
{
    uint32_t addr = 0;
    uint32_t value = 0;

    if (type == 'b' || type == 'w' || type == 'l') { addr = runtime_nextl(runtime); }
    else if (type == 'B' || type == 'W' || type == 'L') { addr = runtime->regs.raw[runtime_nextb(runtime)]; }

    value = runtime->regs.raw[runtime_nextb(runtime)];
}

void runtime_syscall(runtime_t* runtime, bool is_regval)
{
    uint32_t code = 0;
    if (is_regval) { code = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { code = runtime_nextl(runtime); }
    runtime_exec_syscall(runtime, code);
}

void runtime_exec_syscall(runtime_t* runtime, uint32_t code)
{
    for (size_t i = 0; i < sizeof(RUNTIME_SYSCALLS) / sizeof(runtime_syscall_t*); i++)
    {
        const runtime_syscall_t* syscall = RUNTIME_SYSCALLS[i];
        if (syscall->handler != NULL && syscall->code == code)
        {
            debug_log("%s Executing runtime syscall %p\n", DEBUG_INFO, syscall->code);
            syscall->handler(runtime);
            return;
        }
    }   
    debug_error("runtime_exec_syscall(%p, %p) - Invalid runtime syscall", runtime, code);
}

// --------------------------------------------------------------------------------------------------------

// R4 = size(in), R5 = ptr(out)
void runtime_syscall_malloc(runtime_t* runtime)
{
    size_t sz = memalign(runtime->regs.vals.r4, 0x100);

    runtime->regs.vals.r5 = (uint32_t)malloc(sz);
    runtime_map(runtime, runtime->regs.vals.r5, sz, false);

    debug_log("%s syscall_malloc - Address:%p Size:%a\n", DEBUG_INFO, runtime->regs.vals.r5, sz);
}

// R5 = ptr(in)
void runtime_syscall_free(runtime_t* runtime)
{
    void* ptr = (void*)runtime->regs.vals.r5;
    free(ptr);
    runtime_munmap(runtime, (uint32_t)ptr);
    debug_log("%s syscall_free - Address:%p\n", DEBUG_INFO, ptr);
}

// R5 = str(in)
void runtime_syscall_kconsole_write(runtime_t* runtime)
{
    char* str = (char*)runtime->regs.vals.r5;
    if (str == NULL) { debug_log("%s runtime_syscall_kconsole_write(%p) - Attempt to write null string to kconsole\n", DEBUG_WARN, runtime); return; }
    console_write(kconsole_get(), str);
}