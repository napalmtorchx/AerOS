#pragma once
#include <lib/types.h>

typedef enum
{
    RTOP_NOP,
    RTOP_ADD,  RTOP_ADDR,  RTOP_SUB,  RTOP_SUBR,  RTOP_MUL,  RTOP_MULR, RTOP_DIV, RTOP_DIVR,
    RTOP_OR,   RTOP_ORR,   RTOP_XOR,  RTOP_XORR,  RTOP_AND,  RTOP_ANDR, RTOP_NOT,
    RTOP_SHL,  RTOP_SHLR,  RTOP_SHR,  RTOP_SHRR,
    RTOP_JMP,  RTOP_JMPR,  RTOP_CALL, RTOP_CALLR, RTOP_RET,
    RTOP_PUSH, RTOP_PUSHR, RTOP_POP,
    RTOP_SE,   RTOP_SER,   RTOP_SNE,  RTOP_SNER,  RTOP_SGT,  RTOP_SGTR, RTOP_SLT, RTOP_SLTR, RTOP_SC, RTOP_SN, RTOP_SZ,
    RTOP_LD,   RTOP_LDR,   RTOP_LDB,  RTOP_LDW,   RTOP_LDL,  RTOP_LDRB, RTOP_LDRW, RTOP_LDRL,
    RTOP_WRB,  RTOP_WRW,   RTOP_WRL,  RTOP_WRRB,  RTOP_WRRW, RTOP_WRRL,
    RTOP_STB,  RTOP_STW,   RTOP_STL,  RTOP_STRB,  RTOP_STRW, RTOP_STRL,
    RTOP_SYSCALL, RTOP_SYSCALLR,

    RTOP_EXIT = 0xFF,
} RUNTIME_OP;

static const uint8_t RUNTIME_OPCODE_BYTES[] = 
{
    1, 6, 3, 6, 3, 6, 3, 6, 3,
    6, 3, 6, 3, 6, 4, 2,
    3, 3, 3, 3,
    5, 2, 5, 2, 1,
    5, 2, 2,
    6, 3, 6, 3, 6, 3, 6, 3, 2, 2, 2,
    6, 3, 6, 6, 6, 3, 3, 3,
    6, 7, 9, 3, 4, 6,
    6, 6, 6, 3, 3, 3,
    5, 2,
};

typedef struct runtime_t runtime_t;

typedef void (*runtime_syscall_handler_t)(runtime_t* runtime);

typedef struct
{
    char                      name[32];
    uint32_t                  code;
    runtime_syscall_handler_t handler;
} runtime_syscall_t;

void runtime_syscall_malloc(runtime_t* runtime);
void runtime_syscall_free(runtime_t* runtime);
void runtime_syscall_kconsole_write(runtime_t* runtime);

static const runtime_syscall_t RUNTIME_SYSCALL_MALLOC         = (runtime_syscall_t){ "malloc",     0x00000001, runtime_syscall_malloc };
static const runtime_syscall_t RUNTIME_SYSCALL_FREE           = (runtime_syscall_t){ "free",       0x00000002, runtime_syscall_free };
static const runtime_syscall_t RUNTIME_SYSCALL_KCONSOLE_WRITE = (runtime_syscall_t){ "console_wr", 0x00000003, runtime_syscall_kconsole_write };

static const runtime_syscall_t* RUNTIME_SYSCALLS[] = 
{
    &RUNTIME_SYSCALL_MALLOC,
    &RUNTIME_SYSCALL_FREE,
    &RUNTIME_SYSCALL_KCONSOLE_WRITE,
};