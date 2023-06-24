#pragma once
#include <lib/types.h>
#include <lib/kresult.h>
#include <core/memory/memmgr.h>
#include <runtime/executable.h>
#include <runtime/isa.h>

/// Enumerator defining the index of each register value
typedef enum
{
    RTREG_R0, RTREG_R1, RTREG_R2, RTREG_R3, RTREG_R4, RTREG_R5, RTREG_R6, RTREG_R7,
    RTREG_BP, RTREG_SP, RTREG_IP, RTREG_FL, RTREG_COUNT,
} RUNTIME_REG;

/// Structure for managing virtual machine flags
typedef struct
{
    int carry    : 1;       // carry flag - automatically set by arithmetic operations
    int negative : 1;       // negative flag - not automatically set(currently), can me manually set
    int zero     : 1;       // zero flag - automatically set by arithmetic operations
} runtime_flagvals_t;

/// Union used to access virtual machine flags as both a structure or raw register value
typedef union
{
    runtime_flagvals_t vals;    // flags structure
    uint32_t           raw;     // raw 32-bit integer value
} runtime_flags_t;

// Structure used for managing register values
typedef struct
{
    uint32_t        r0, r1, r2, r3, r4, r5, r6, r7;   // general purpose registers
    uint32_t        bp, sp, ip;                       // address registers
    runtime_flags_t flags;                            // flags register
} runtime_regvals_t;

// Union used to access virtual machine registers as both a structure and an array for 32-bit integer values
typedef union
{
    runtime_regvals_t vals;
    uint32_t          raw[RTREG_COUNT];
} runtime_regs_t;

typedef struct runtime_t runtime_t;

// Virtual machine runtime environment
struct runtime_t
{
    runtime_regs_t regs;                // Register values
    executable_t*  exec;                // Pointer to program executable - should be start of 'program'
    uint8_t*       program;             // Pointer to raw program data
    size_t         programsz;           // Size of raw program data
    uint32_t*      stack;               // Pointer to virtual machine stack
    size_t         stacksz;             // Size of virtual machine stack
    memblk_t*      mmap;                // Pointer to memory map entries - virtual machine should only be able to access memory mapped here
    size_t         mmap_count;          // Amount of memory map entries
    bool           running;             // Running flag - 'runtime_run' will terminate with this is not set
};

/// @brief Create new virtual machine instance @param program Pointer to raw program data @param progsz Size of raw program data @param stacksz Size of virtual machine stack @return Virtual machine runtime structure
runtime_t runtime_create(uint8_t* program, size_t progsz, size_t stacksz);

/// @brief Execute the program in a loop until the 'EXIT' instruction has be executed or an error has occured @param runtime Pointer to virtual machine
void runtime_run(runtime_t* runtime);

/// @brief Execute a single instruction and increment the program counter @param runtime Pointer to virtual machine
void runtime_step(runtime_t* runtime);

/// @brief Map a region of memory for the virtual machine to have access to @param runtime Pointer to virtual machine @param addr Staring address of memory region @param sz Size of memory region @param readonly Is memory readonly or read/write @return Pointer to the memory block stored in runtime mmap
memblk_t* runtime_map(runtime_t* runtime, uint32_t addr, size_t sz, bool readonly);

/// @brief Unmap a region of memory based on staring address @param runtime Pointer to virtual machine @param addr Starting address of memory region
void runtime_munmap(runtime_t* runtime, uint32_t addr);

/// @brief Check if a region of memory starting at address has been mapped @param runtime Pointer to virtual machine @param addr Staring address of memory region @return Is memory mapped
bool runtime_ismapped(runtime_t* runtime, uint32_t addr);

/// @brief Read data from memory at IP and increment by 1 byte @param runtime Pointer to virtual machine @return Value read from memory
uint8_t runtime_nextb(runtime_t* runtime);

/// @brief Read data from memory at IP and increment by 2 bytes @param runtime Pointer to virtual machine @return Value read from memory
uint16_t runtime_nextw(runtime_t* runtime);

/// @brief Read data from memory at IP and increment by 4 bytes @param runtime Pointer to virtual machine @return Value read from memory
uint32_t runtime_nextl(runtime_t* runtime);

/// @brief Read byte from memory at specified address @param runtime Pointer to virtual machine @param addr Address to read from @return 8-bit value at address
uint8_t runtime_rdb(runtime_t* runtime, uint32_t addr);

/// @brief Read 2 bytes from memory at specified address @param runtime Pointer to virtual machine @param addr Address to read from @return 16-bit value at address
uint16_t runtime_rdw(runtime_t* runtime, uint32_t addr);

/// @brief Read 4 bytes from memory at specified address @param runtime Pointer to virtual machine @param addr Address to read from @return 32-bit value at address
uint32_t runtime_rdl(runtime_t* runtime, uint32_t addr);

/// @brief Write 1 byte to memory at specified address @param runtime Pointer to virtual machine @param addr Address to write to @param value 8-bit value to write
void runtime_wrb(runtime_t* runtime, uint32_t addr, uint8_t value);

/// @brief Write 2 bytes to memory at specified address @param runtime Pointer to virtual machine @param addr Address to write to @param value 16-bit value to write
void runtime_wrw(runtime_t* runtime, uint32_t addr, uint16_t value);

/// @brief Write 4 bytes to memory at specified address @param runtime Pointer to virtual machine @param addr Address to write to @param value 32-bit value to write
void runtime_wrl(runtime_t* runtime, uint32_t addr, uint32_t value);

/// @brief Check if region of memory is accessible by the virtual machine and within mmap bounds, returns error on failure @param runtime Pointer to virtual machine @param addr Staring address of memory region @param sz Size of region to check @param wr Is memory writable or read-only?
void runtime_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr);

/// @brief Check if region of memory is accessible by the virtual machine and within mmap bounds, returns result instead of error @param runtime Pointer to virtual machine @param addr Staring address of memory region @param sz Size of region to check @param wr Is memory writable or read-only? @return Result of memory check
KRESULT runtime_try_checkaddr(runtime_t* runtime, uint32_t addr, size_t sz, bool wr);
