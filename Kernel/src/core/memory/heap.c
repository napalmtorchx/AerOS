#include <core/memory/heap.h>
#include <kernel.h>

heap_t heap_create(uintptr_t base, uintptr_t alloc_stack_base) {
	// initialize the heap
    return (heap_t) { base, (alloc_entry_t *)alloc_stack_base, 0 };
}
heap_t init_kernel_heap(bool dbg_msgs) {
    uint32_t total = memmgr_amount_installed(0);
    uint32_t sz = total-(total/8);
    
    // require memory block and create heap
    memblk_t *memblock = memmgr_alloc(sz, MEM_HEAP);
    heap_t kheap = heap_create((uintptr_t)memblock->addr, sz);
    kheap.totalmem = sz;
    kheap.debug_msgs = dbg_msgs;

    debug_log(DEBUG_INFO " HEAP BASE:%8x ALLOC_STACK:%8x\n", kheap.base, kheap.alloc_stack_base);

    return kheap;
}

alloc_entry_t heap_push_entry(heap_t *heap, alloc_entry_t entry) {
    if (heap->alloc_entries_count != 0) {
        alloc_entry_t prev_entry = *(heap->alloc_stack_base);
        entry.offset_start = prev_entry.offset_end;
        entry.offset_end += entry.offset_start;
    } else {
        entry.offset_start = 0;
    }
  
    // push entry into stack
	*(--heap->alloc_stack_base) = entry;
    heap->alloc_entries_count++;
  
	return entry;
}
alloc_entry_t heap_insert_entry(heap_t *heap, alloc_entry_t entry, uintptr_t index) {
    // increase stack space by 1 entry
	heap->alloc_stack_base--;
  
	// make space at given index
	for (uintptr_t i = 0; i < index; i++)
    heap->alloc_stack_base[i] = heap->alloc_stack_base[i + 1];
  
    if (index != 0) {
        entry.offset_start = heap->alloc_stack_base[index + 1].offset_end;
        entry.offset_end += entry.offset_start;
    }

	// set new entry
    heap->alloc_stack_base[index] = entry;
    heap->alloc_entries_count++;
  
	return entry;
}
bool heap_delete_entry(heap_t *heap, size_t index) {
  if (index >= heap->alloc_entries_count) return false;
  
	// overwrite entry
	for (size_t i = index; i > 0; i--) {
        heap->alloc_stack_base[i] = heap->alloc_stack_base[i - 1];
    }

	// reduce stack space by 1 entry
	heap->alloc_stack_base++;
    heap->alloc_entries_count--;
  
	return true;
}
size_t heap_find_entry_index(heap_t *heap, alloc_entry_t entry) {
    for (uintptr_t i = 0; i < heap->alloc_entries_count; i++) { 
        // if offsets start and end correspond to the given ones, return the found index
        if (heap->alloc_stack_base[i].offset_start == entry.offset_start &&
            heap->alloc_stack_base[i].offset_end   == entry.offset_end) return i;
    }
  
	return -1;
}
alloc_entry_t heap_find_entry_from_address(heap_t *heap, uintptr_t address) {
    for (uintptr_t i = 0; i < heap->alloc_entries_count; i++) {
        // if the entry contains the given address, return the found entry
        if (heap->alloc_stack_base[i].offset_start <= address &&
            heap->alloc_stack_base[i].offset_end   > address)
                return heap->alloc_stack_base[i];
    }
  
	return (alloc_entry_t) {
    .offset_end = 0xFFFFFFFF
  };
}

alloc_entry_t heap_alloc(
    heap_t *heap,
    size_t sz,
    alloc_type_t type,
    alloc_data_type_t data_type) {
    
    // create the template entry
    alloc_entry_t entry = (alloc_entry_t) {
        .offset_start = INT32_MAX,
        .offset_end = sz,
        .data_type = data_type,
        .type = type
    };

    // push new entry if the allocation stack is not populated
    if (heap->alloc_entries_count == 0 || heap->alloc_entries_count == 1)
        entry = heap_push_entry(heap, entry);

    if (entry.offset_start != INT32_MAX) goto alloc_end;

    // check for available space
    uint32_t count;
    for (count = 0; count < heap->alloc_entries_count; count++)
    {
        alloc_entry_t entry = heap->alloc_stack_base[count];
        alloc_entry_t next_entry = heap->alloc_stack_base[count + 1];

        if (entry.offset_start - next_entry.offset_end >= sz) break;
    }

    if (count == heap->alloc_entries_count) {
        // push new entry if we hit stack top
        entry = heap_push_entry(heap, entry);
    } else {
        // insert entry between available space
        entry = heap_insert_entry(heap, entry, count + 1);
    }

    alloc_end:

    if (heap->debug_msgs)
    {
        debug_log(DEBUG_MALLOC " ADDR:%8x-%8x SIZE:%a(%a)\n",
              entry.offset_start + heap->base,
              entry.offset_end + heap->base, 
              entry.offset_end - entry.offset_start,
              sz);
    }

    return entry;
}
void heap_free(heap_t *heap, alloc_entry_t entry) {
    if (heap->alloc_entries_count == 0) {
       debug_error(DEBUG_FREE " INVALID FREE REQUEST!");
       return;
    }
    
    heap_delete_entry(heap, heap_find_entry_index(heap, entry));

    if (heap->debug_msgs)
    {
        debug_log(DEBUG_FREE " ADDR:%8x-%8x SIZE:%a\n",
              entry.offset_start + heap->base,
              entry.offset_end + heap->base,
              entry.offset_end - entry.offset_start);
    }
}
alloc_entry_t heap_get_alloc_info(heap_t *heap, uintptr_t addr) {
    // locate entry
    alloc_entry_t entry = heap_find_entry_from_address(heap, addr);
    return entry;
}
size_t heap_get_used_mem(heap_t *heap) {
    size_t total = 0;

    // count the total used memory
    for (int i = 0; i < heap->alloc_entries_count; i++) {
        total += heap->alloc_stack_base[i].offset_end - heap->alloc_stack_base[i].offset_start; 
    }
    return total;
}

size_t heap_get_total_mem(heap_t *heap) { return heap->totalmem; }

char *heap_convert_type(alloc_type_t type) {
    switch (type) {
        case HEAP_ALLOC_TYPE_DIRECT:    return "direct";
        case HEAP_ALLOC_TYPE_NEW:       return "new operator";
        case HEAP_ALLOC_TYPE_NEW_ARRAY: return "new[] operator";
        
        default: return "unknown";
    }
}
char *heap_convert_data_type(alloc_data_type_t data_type) {
    switch (data_type) {
        case HEAP_ALLOC_DATA_TYPE_OTHER:    return "other";
        case HEAP_ALLOC_DATA_TYPE_INTEGER:  return "integer";
        case HEAP_ALLOC_DATA_TYPE_STRING:   return "string";
        case HEAP_ALLOC_DATA_TYPE_ARRAY:    return "array";
    
        default: return "unknown";
    }
}