#pragma once
#include <lib/types.h>

// Enumerator for virtual machine opcode identification - Value is equal to opcode
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

// Amount of bytes for used when executing opcodes - index is equal to the opcode(RUNTIME_OP)
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

// Function handler for virtual machine system call
typedef void (*runtime_syscall_handler_t)(runtime_t* runtime);

// Structure for managing a virtual machine system call
typedef struct
{
    char                      name[32];             // name of system call
    uint32_t                  code;                 // Code used to identify system call
    runtime_syscall_handler_t handler;              // Pointer to function for handling system call
} runtime_syscall_t;

/// @brief Allocate memory for use in virtual machine - R4(in):Size R5(out):Address @param runtime Pointer to virtual machine
void runtime_syscall_malloc(runtime_t* runtime);

/// @brief Free allocated memory used in virtual machine - R5(in):Address @param runtime Pointer to virtual machine
void runtime_syscall_free(runtime_t* runtime);

/// @brief Print a string to the kernel console - R5(in):Address @param runtime Pointer to virtual machine
void runtime_syscall_kconsole_write(runtime_t* runtime);

// Virtual machine system call - 'malloc'
static const runtime_syscall_t RUNTIME_SYSCALL_MALLOC         = (runtime_syscall_t){ "malloc",     0x00000001, runtime_syscall_malloc };

// Virtual machine system call - 'free'
static const runtime_syscall_t RUNTIME_SYSCALL_FREE           = (runtime_syscall_t){ "free",       0x00000002, runtime_syscall_free };

// Virtual machine system call - 'console_wr'
static const runtime_syscall_t RUNTIME_SYSCALL_KCONSOLE_WRITE = (runtime_syscall_t){ "console_wr", 0x00000003, runtime_syscall_kconsole_write };

// List of all registered virtual machine system calls
static const runtime_syscall_t* RUNTIME_SYSCALLS[] = 
{
    &RUNTIME_SYSCALL_MALLOC,
    &RUNTIME_SYSCALL_FREE,
    &RUNTIME_SYSCALL_KCONSOLE_WRITE,
};

/// @brief Perform arithmetic operation on immediate value @param runtime Pointer to virtual machine @param type Type of arithmetic operation
void runtime_arith_imm(runtime_t* runtime, char type);

/// @brief Perform arithmetic operation on 2 registers @param runtime Pointer to virtual machine @param type Type of arithmetic operation
void runtime_arith_reg(runtime_t* runtime, char type);

/// @brief Perform arithmetic operation, reads arguments from memory and increments IP @param runtime Pointer to virtual machine @param is_regval Is second argument immediate or register @param type Type of arithmetic operation, defined as char. i.e. '+', '-', '*', etc
void runtime_arith(runtime_t* runtime, bool is_regval, char type);

/// @brief Jump to address, reads argument from memory and increments IP @param runtime Pointer to virtual machine @param is_regval Is address immediate value or stored in register
void runtime_jump(runtime_t* runtime, bool is_regval);

/// @brief Call subroutine, reads argument from memory and increments IP @param runtime Pointer to virtual machine @param is_regval Is address immediate value or stored in register
void runtime_call(runtime_t* runtime, bool is_regval);

/// @brief Pop value from stack, reads argument from memory and increments IP @param runtime Pointer to virtual machine @param reg Register to pop value into
void runtime_pop(runtime_t* runtime, uint8_t reg);

/// @brief Push value onto stack, reads argument from memory and increments IP @param runtime Pointer to virtual machine @param is_regval Is argument immediate or register
void runtime_push(runtime_t* runtime, bool is_regval);

/// @brief Skip next instruction if condition is met, reads arguments from memory and increments IP @param runtime Pointer to virtual machine @param is_regval Is second argument immediate or register @param type Type of skip, defined as char. i.e. '=', '!', '<', '>', 'c', 'z', 'n'
void runtime_skip(runtime_t* runtime, bool is_regval, char type);

/// @brief Load value into register, reads arguments from memory and increments IP @param runtime Pointer to virtual machine @param type Type of load method, defined as char. i.e. 'i' - immediate, 'r' - register, 'b/w/l' - value at immediate address, 'B/W/L' - value at address in register
void runtime_ldreg(runtime_t* runtime, char type);

/// @brief Write immediate value into memory, reads arguments from memory and increments IP @param runtime Pointer to virtual machine @param type Type of load method, defined as char. i.e. 'i' - immediate, 'r' - register, 'b/w/l' - value at immediate address, 'B/W/L' - value at address in register
void runtime_wrreg(runtime_t* runtime, char type);

/// @brief Write value from register into memory, reads arguments from memory and increments IP @param runtime Pointer to virtual machine @param type Type of load method, defined as char. i.e. 'i' - immediate, 'r' - register, 'b/w/l' - value at immediate address, 'B/W/L' - value at address in register
void runtime_streg(runtime_t* runtime, char type);

/// @brief Execute system call, reads argument from memory and increments IP @param runtime Pointer to virtual machine @param is_regval Is argument immediate or register
void runtime_syscall(runtime_t* runtime, bool is_regval);

/// @brief Execute system call @param runtime Pointer to virtual machine @param code Virtual machine system call code to execute
void runtime_exec_syscall(runtime_t* runtime, uint32_t code);