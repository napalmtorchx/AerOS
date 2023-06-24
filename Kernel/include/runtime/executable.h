#pragma once
#include <lib/types.h>

// type of executable - service can access crucial system functions, but applications are somewhat restricted
typedef enum
{
    EXEC_INVALID,           // invalid executable type
    EXEC_SERVICE,           // service/driver
    EXEC_APPLICATION,       // applcation/game/utility
} EXEC_TYPE;

// Virtual machine executable/program header
typedef struct
{
    char      name[32];             // name of executable
    uint32_t  type;                 // type of executable
    uint32_t  text_offset;          // offset of code
    uint32_t  text_sz;              // size of code
    uint32_t  data_offset;          // offset of data
    uint32_t  data_sz;              // size of data
} attr_pack executable_t;