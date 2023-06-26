#pragma once

#include <lib/types.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

typedef enum alloc_data_type {
  HEAP_ALLOC_DATA_TYPE_OTHER,
  HEAP_ALLOC_DATA_TYPE_INTEGER,
  HEAP_ALLOC_DATA_TYPE_STRING,
  HEAP_ALLOC_DATA_TYPE_ARRAY
} alloc_data_type_t;
typedef enum alloc_type {
  HEAP_ALLOC_TYPE_DIRECT,
  HEAP_ALLOC_TYPE_NEW,
  HEAP_ALLOC_TYPE_NEW_ARRAY
} alloc_type_t;

typedef struct alloc_entry {
  uintptr_t offset_start;
  uintptr_t offset_end;
  alloc_data_type_t data_type : 4;
  alloc_type_t type : 4;
} attr_pack alloc_entry_t;

typedef struct heap {
  uintptr_t base;
  alloc_entry_t *alloc_stack_base;
  uint32_t alloc_entries_count;
  size_t totalmem;
  bool debug_msgs;
}   heap_t;

heap_t heap_create(uintptr_t base, uintptr_t alloc_stack_base);

alloc_entry_t heap_alloc(
    heap_t *heap,
    size_t sz,
    alloc_type_t type,
    alloc_data_type_t data_type);
void heap_free(heap_t *heap, alloc_entry_t entry);
alloc_entry_t heap_get_alloc_info(heap_t *heap, uintptr_t addr);

size_t heap_get_used_mem(heap_t *heap);
size_t heap_get_total_mem(heap_t *heap);

heap_t init_kernel_heap(bool dbg_msgs);