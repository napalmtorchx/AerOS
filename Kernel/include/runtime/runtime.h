#pragma once
#include <lib/types.h>
#include <lib/kresult.h>
#include <core/memory/memmgr.h>
#include <runtime/executable.h>
#include <runtime/isa.h>

typedef enum
{
    RTREG_R0, RTREG_R1, RTREG_R2, RTREG_R3, RTREG_R4, RTREG_R5, RTREG_R6, RTREG_R7,
    RTREG_BP, RTREG_SP, RTREG_IP, RTREG_FL, RTREG_COUNT,
} RUNTIME_REG;

typedef struct
{
    int carry    : 1;
    int negative : 1;
    int zero     : 1;
} runtime_flagvals_t;

typedef union
{
    runtime_flagvals_t vals;
    uint32_t           raw;
} runtime_flags_t;

typedef struct
{
    uint32_t        r0, r1, r2, r3, r4, r5, r6, r7;
    uint32_t        bp, sp, ip;
    runtime_flags_t flags;
} runtime_regvals_t;

typedef union
{
    runtime_regvals_t vals;
    uint32_t          raw[RTREG_COUNT];
} runtime_regs_t;

typedef struct runtime_t runtime_t;
struct runtime_t
{
    runtime_regs_t regs;
    executable_t*  exec;
    uint8_t*       program;
    size_t         programsz;
    uint32_t*      stack;
    size_t         stacksz;
    memblk_t*      mmap;
    size_t         mmap_count;
    bool           running;
};

runtime_t runtime_create(uint8_t* program, size_t progsz, size_t stacksz);
void runtime_run(runtime_t* runtime);
void runtime_step(runtime_t* runtime);
void runtime_exec_syscall(runtime_t* runtime, uint32_t code);

memblk_t* runtime_map(runtime_t* runtime, uint32_t addr, size_t sz, bool readonly);
void      runtime_munmap(runtime_t* runtime, uint32_t addr);
bool      runtime_ismapped(runtime_t* runtime, uint32_t addr);

uint8_t  runtime_nextb(runtime_t* runtime);
uint16_t runtime_nextw(runtime_t* runtime);
uint32_t runtime_nextl(runtime_t* runtime);

uint8_t  runtime_rdb(runtime_t* runtime, uint32_t addr);
uint16_t runtime_rdw(runtime_t* runtime, uint32_t addr);
uint32_t runtime_rdl(runtime_t* runtime, uint32_t addr);

void runtime_wrb(runtime_t* runtime, uint32_t addr, uint8_t value);
void runtime_wrw(runtime_t* runtime, uint32_t addr, uint16_t value);
void runtime_wrl(runtime_t* runtime, uint32_t addr, uint32_t value);

void runtime_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr);
KRESULT runtime_try_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr);
