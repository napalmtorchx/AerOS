#pragma once
#include <lib/types.h>
#include <core/memory/memmgr.h>

#define HEAP_ALIGN 0x1000
#define HEAP_COUNT 32768

typedef enum
{
    HEAPSTATE_INVALID,
    HEAPSTATE_FREE,
    HEAPSTATE_USED,
} HEAPSTATE;

typedef struct
{
    uintptr_t addr;
    size_t    sz;
    HEAPSTATE state;
    void*     thread;
} attr_pack heapblk_t;

void heap_init(void);
void heap_print(bool cmd);

void* heap_alloc(size_t sz, bool zerofill);
void heap_free(void* ptr);
size_t heap_collect(void);
bool heap_merge(heapblk_t* blk1, heapblk_t* blk2);

heapblk_t* heap_request(size_t sz);
heapblk_t* heap_next(void);
heapblk_t* heap_nearest(heapblk_t* blk);
heapblk_t* heap_create(heapblk_t blk);
void       heap_remove(heapblk_t* blk);

heapblk_t* heap_from_addr(void* ptr);
heapblk_t* heap_from_index(int index);

bool heap_validate(heapblk_t* blk);

size_t heap_count_type(HEAPSTATE state);
size_t heap_amount_total(void);
size_t heap_amount_free(void);
size_t heap_amount_used(void);

const char* heap_statestr(HEAPSTATE state);



