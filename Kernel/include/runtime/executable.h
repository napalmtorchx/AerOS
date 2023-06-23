#pragma once
#include <lib/types.h>

typedef enum
{
    EXEC_INVALID,
    EXEC_SERVICE,
    EXEC_APPLICATION,
} EXEC_TYPE;

typedef struct
{
    char      name[32];
    EXEC_TYPE type;
    uint32_t  text_offset;
    uint32_t  text_sz;
    uint32_t  data_offset;
    uint32_t  data_sz;
} attr_pack executable_t;