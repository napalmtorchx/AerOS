#include <runtime/isa.h>
#include <kernel.h>

void runtime_arith_imm(runtime_t* runtime, char type)
{
    // use a 64-bit value to store the result, in-case the sum is larger and requires a carry
    uint64_t result = 0;

    // fetch first argument from memory - for arithmetic operations, the first argument is always a register
    uint8_t reg = runtime_nextb(runtime);

    // fetch second argument from memory - immediate 32-bit value unless it is a shift operation, which only requires 1 byte
    uint32_t value  = 0;
    if (type == '<' || type == '>') { value = runtime_nextb(runtime); }
    else { value = runtime_nextl(runtime); }

    // check type of arithmetic and perform operation
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

    // set destination register to result of operation
    runtime->regs.raw[reg] = (uint32_t)result;
    // set cpu flags based on result of operation
    runtime_setflags(runtime, result);
}

void runtime_arith_reg(runtime_t* runtime, char type)
{
    // use a 64-bit value to store the result, in-case the sum is larger and requires a carry
    uint64_t result = 0;

    // fetch first argument from memory - for arithmetic operations, the first argument is always a register 
    uint8_t rx = runtime_nextb(runtime);

    // fetch second argument from memory - register value
    uint8_t ry = runtime_nextb(runtime);

    // check type of arithmetic and perform operation
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

    // set destination register to result of operation
    runtime->regs.raw[rx] = (uint32_t)result;
    // set cpu flags based on result of operation
    runtime_setflags(runtime, result);
}

void runtime_arith(runtime_t* runtime, bool is_regval, char type)
{
    // arithmetic operations with 2 arguments
    if (type == '+' || type == '-' || type == '*' || type == '/' || type == '|' || type == '^' || type == '&' || type == '<' || type == '>')
    {
        if (is_regval) { runtime_arith_reg(runtime, type); }
        else           { runtime_arith_imm(runtime, type); }
        return;
    }
    // arithmetic operations with 1 argument - currently only the NOT instruction
    else if (type == '~')
    {
        uint8_t reg = runtime_nextb(runtime);
        runtime->regs.raw[reg] = ~runtime->regs.raw[reg];
        return;
    }

    // Failed to determine type of artithmetic operation
    debug_error("runtime_arith(%p, %d, '%c') - Invalid operation '%c'", runtime, is_regval, type, type);
}

void runtime_jump(runtime_t* runtime, bool is_regval)
{
    // value used to store address to jump to
    uint32_t addr = 0;

    // check if argument is stored in a register or is immediate
    if (is_regval) { addr = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { addr = runtime_nextl(runtime); }

    // jump to address in memory
    runtime->regs.vals.ip = addr;
}

void runtime_call(runtime_t* runtime, bool is_regval)
{
    // validate that stack pointer is within stack bounds
    if (runtime->regs.vals.sp <  (uint32_t)runtime->stack)                    { debug_error("runtime_call(%p, %d) - Stack underflow", runtime, is_regval); return; }
    if (runtime->regs.vals.sp >= (uint32_t)runtime->stack + runtime->stacksz) { debug_error("runtime_call(%p, %d) - Stack overflow",  runtime, is_regval); return; }

    // check if argument is stored in a register or is immediate
    uint32_t addr = 0;
    if (is_regval) { addr = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { addr = runtime_nextl(runtime); }
    
    // detemine the next instructions amount of bytes
    uint8_t next_op    = runtime_rdb(runtime, runtime->regs.vals.ip);
    uint8_t next_bytes = RUNTIME_OPCODE_BYTES[next_op];

    // write the address of the next instruction to the stack and increment stack pointer
    runtime_wrl(runtime, runtime->regs.vals.sp, next_bytes);
    runtime->regs.vals.sp += 4;

    // jump to address in memory
    runtime->regs.vals.ip = addr;
}

void runtime_pop(runtime_t* runtime, uint8_t reg)
{
    // decrement stack pointer
    runtime->regs.vals.sp -= 4;

    // validate stack pointer address before continuing
    if (runtime->regs.vals.sp < (uint32_t)runtime->stack) { debug_error("runtime_pop(%p, %2x) - Stack underflow", runtime, reg); return; }

    // pop value from the stack into desired register
    runtime->regs.raw[reg] = runtime_rdl(runtime, runtime->regs.vals.sp);
}

void runtime_push(runtime_t* runtime, bool is_regval)
{
    // validate that stack pointer is within stack bounds
    if (runtime->regs.vals.sp <  (uint32_t)runtime->stack)                    { debug_error("runtime_push(%p, %d) - Stack underflow", runtime, is_regval); return; }
    if (runtime->regs.vals.sp >= (uint32_t)runtime->stack + runtime->stacksz) { debug_error("runtime_push(%p, %d) - Stack overflow",  runtime, is_regval); return; }

    // fetch argument from memory - could be either immediate 32-bit value or a register
    uint32_t val = 0;
    if (is_regval) { val = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { val = runtime_nextl(runtime); }

    // write value to stack and increment stack pointer
    runtime_wrl(runtime, runtime->regs.vals.sp, val);
    runtime->regs.vals.sp += 4;
}

void runtime_skip(runtime_t* runtime, bool is_regval, char type)
{
    // get first argument of skip operation - always a register unless it is a skip-flag operation(SC, SZ, SN), in that case it is boolean
    uint8_t arg0 = runtime_nextb(runtime);

    // get second argument of skip operation
    uint32_t val = 0;

    // if skip operation is for flags, the argument is a boolean value
    if (type == 'c' || type == 'n' || type == 'z') { val = runtime_nextb(runtime); }
    // otherwise check if it is a register value
    else if (is_regval) { val = runtime->regs.raw[runtime_nextb(runtime)]; }
    // or an immediate 32-bit value
    else { val = runtime_nextl(runtime); }

    // detemine the next instructions amount of bytes
    uint8_t next_op    = runtime_rdb(runtime, runtime->regs.vals.ip);
    uint8_t next_bytes = RUNTIME_OPCODE_BYTES[next_op];

    // determine if skip condition has been met based on type
    bool cond = false;
    if (type == '=') { cond = (runtime->regs.raw[arg0] == val); }
    if (type == '!') { cond = (runtime->regs.raw[arg0] != val); }
    if (type == '<') { cond = (runtime->regs.raw[arg0] < val); }
    if (type == '>') { cond = (runtime->regs.raw[arg0] > val); }
    if (type == 'c') { cond = (runtime->regs.vals.flags.vals.carry    == arg0); }
    if (type == 'z') { cond = (runtime->regs.vals.flags.vals.zero     == arg0); }
    if (type == 'n') { cond = (runtime->regs.vals.flags.vals.negative == arg0); }

    // if condition has been met, skip the next instruction
    if (cond) { runtime->regs.vals.ip += next_bytes; }
}

void runtime_ldreg(runtime_t* runtime, char type)
{
    // fetch first argument which is a register value
    uint8_t reg = runtime_nextb(runtime);

    // create variable to store second arguments immediate value
    uint32_t value = 0;

    // second argument is 32-bit immediate value
    if (type == 'i') { value = runtime_nextl(runtime); }
    // second argument is another register
    else if (type == 'r') { value = runtime->regs.raw[runtime_nextb(runtime)]; }
    // second argument is 1-byte at specified immediate address
    else if (type == 'b') { value = runtime_rdb(runtime, runtime_nextl(runtime)); }
    // second argument is 2-bytes at specified immediate address
    else if (type == 'w') { value = runtime_rdw(runtime, runtime_nextl(runtime)); }
    // second argument is 4-bytes at specified immediate address
    else if (type == 'l') { value = runtime_rdl(runtime, runtime_nextl(runtime)); }
    // second argument is 1-byte at address stored in register
    else if (type == 'B') { value = runtime_rdb(runtime, runtime->regs.raw[runtime_nextb(runtime)]); }
    // second argument is 2-bytes at address stored in register
    else if (type == 'W') { value = runtime_rdw(runtime, runtime->regs.raw[runtime_nextb(runtime)]); }
    // second argument is 4-byts at address stored in register
    else if (type == 'L') { value = runtime_rdl(runtime, runtime->regs.raw[runtime_nextb(runtime)]); }

    // set destination register value
    runtime->regs.raw[reg] = value;
}

void runtime_wrreg(runtime_t* runtime, char type)
{
    // create variables used to store arguments of instruction
    uint32_t addr = 0, value = 0;

    // write an immediate 8-bit value to memory at immediate address
    if (type == 'b')
    {
        addr  = runtime_nextl(runtime);
        value = runtime_nextb(runtime);
        runtime_wrb(runtime, addr, (uint8_t)value);
    }
    // write an immediate 16-bit value to memory at immediate address
    else if (type == 'w')
    {
        addr  = runtime_nextl(runtime);
        value = runtime_nextw(runtime);
        runtime_wrw(runtime, addr, (uint16_t)value);
    }
    // write an immediate 32-bit value to memory at immediate address
    else if (type == 'l')
    {
        addr  = runtime_nextl(runtime);
        value = runtime_nextl(runtime);
        runtime_wrw(runtime, addr, value);
    }
    // write an immediate 8-bit value to memory at address in register
    else if (type == 'B')
    {
        addr  = runtime->regs.raw[runtime_nextb(runtime)];
        value = runtime_nextb(runtime);
        runtime_wrb(runtime, addr, (uint8_t)value);
    }
    // write an immediate 16-bit value to memory at address in register
    else if (type == 'W')
    {
        addr  = runtime->regs.raw[runtime_nextb(runtime)];
        value = runtime_nextw(runtime);
        runtime_wrw(runtime, addr, (uint16_t)value);
    }
    // write an immediate 32-bit value to memory at address in register
    else if (type == 'L')
    {
        addr  = runtime->regs.raw[runtime_nextb(runtime)];
        value = runtime_nextl(runtime);
        runtime_wrw(runtime, addr, value);
    }
}

void runtime_streg(runtime_t* runtime, char type)
{
    // fetch first argument which is a register value
    uint32_t addr = 0, value = 0;

    // first argument is an immediate address value
    if (type == 'b' || type == 'w' || type == 'l') { addr = runtime_nextl(runtime); }
    // first argument is a register containing the address value
    else if (type == 'B' || type == 'W' || type == 'L') { addr = runtime->regs.raw[runtime_nextb(runtime)]; }

    // second argument is the value store in the register
    value = runtime->regs.raw[runtime_nextb(runtime)];

    // write value to memory
    if (type == 'b' || type == 'B') { runtime_wrb(runtime, addr, (uint8_t)value);  return; }
    if (type == 'w' || type == 'W') { runtime_wrw(runtime, addr, (uint16_t)value); return; }
    if (type == 'l' || type == 'L') { runtime_wrl(runtime, addr, (uint32_t)value); return; }
}

void runtime_syscall(runtime_t* runtime, bool is_regval)
{
    // check if argument is an immediate or register value
    uint32_t code = 0;
    if (is_regval) { code = runtime->regs.raw[runtime_nextb(runtime)]; }
    else { code = runtime_nextl(runtime); }

    // execute specified system call
    runtime_exec_syscall(runtime, code);
}

void runtime_exec_syscall(runtime_t* runtime, uint32_t code)
{
    // iterate through list of system call structures
    for (size_t i = 0; i < sizeof(RUNTIME_SYSCALLS) / sizeof(runtime_syscall_t*); i++)
    {
        const runtime_syscall_t* syscall = RUNTIME_SYSCALLS[i];

        // located valid matching system call 
        if (syscall->handler != NULL && syscall->code == code)
        {
            // execute system call and return
            debug_log("%s Executing runtime syscall %p\n", DEBUG_INFO, syscall->code);
            syscall->handler(runtime);
            return;
        }
    }   
    // invalid virtual machine system call
    debug_error("runtime_exec_syscall(%p, %p) - Invalid runtime syscall", runtime, code);
}

// --------------------------------------------------------------------------------------------------------

// R4 = size(in), R5 = ptr(out)
void runtime_syscall_malloc(runtime_t* runtime)
{
    // get size from register(R4)
    size_t sz = memalign(runtime->regs.vals.r4, 0x100);

    // set register(R5) to the pointer return from malloc function
    runtime->regs.vals.r5 = (uint32_t)malloc(sz);

    // map allocated memory into mmap for virtual machine to access
    runtime_map(runtime, runtime->regs.vals.r5, sz, false);

    // log virtual machine allocation information
    debug_log("%s syscall_malloc - Address:%p Size:%a\n", DEBUG_INFO, runtime->regs.vals.r5, sz);
}

// R5 = ptr(in)
void runtime_syscall_free(runtime_t* runtime)
{
    // get pointer to free from register(R5)
    void* ptr = (void*)runtime->regs.vals.r5;
    
    // attemp to free pointer
    free(ptr);

    // unmap allocated memory from mmap
    runtime_munmap(runtime, (uint32_t)ptr);

    // log virtual machine free information
    debug_log("%s syscall_free - Address:%p\n", DEBUG_INFO, ptr);
}

// R5 = str(in)
void runtime_syscall_kconsole_write(runtime_t* runtime)
{
    // get string pointer from register(R5)
    char* str = (char*)runtime->regs.vals.r5;

    // attempt to draw null string
    if (str == NULL) { debug_log("%s runtime_syscall_kconsole_write(%p) - Attempt to write null string to kconsole\n", DEBUG_WARN, runtime); return; }

    // draw string value to kernel console
    console_write(kconsole_get(), str);
}