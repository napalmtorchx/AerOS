#include <core/memory/memmgr.h>
#include <kernel.h>

#define MEMBLK_COUNT 2048

static memblk_t  _memblks[MEMBLK_COUNT];
static size_t    _count;
static memblk_t* _fblk;

void memmgr_scan_mmap()
{
    mmap_entry_t* mmap = mboot_get()->mmap_addr;
    size_t i, count = mboot_get()->mmap_length / sizeof(mmap_entry_t);
    size_t ksz = kernel_size();

_do_memscan:
    for (i = 0; i < count; i++)
    {
        if (mmap[i].size == 0) { continue; }
        if (mmap[i].type == MEM_INVALID || mmap[i].type == MEM_RESERVED) { continue; }
        if (mmap[i].address < kernel_addr_start() || mmap[i].address + mmap[i].size > 0xFFFFFFFF) { continue; }
        if (mmap[i].address == kernel_addr_start())
        {
            memmgr_map((uintptr_t)mmap[i].address, ksz, MEM_KERNEL);
            _fblk = memmgr_map((uintptr_t)mmap[i].address + ksz, (uint32_t)mmap[i].size - ksz, MEM_FREE);
            mmap[i] = (mmap_entry_t){ 0, 0, 0, 0 };
            goto _do_memscan;
        }

        memmgr_map((uintptr_t)mmap[i].address, (size_t)mmap[i].size, (MEMTYPE)mmap[i].type);
        mmap[i] = (mmap_entry_t){ 0, 0, 0, 0 };
    }

    debug_log("%s Finished scanning mmap entries - %u found\n", DEBUG_INFO, _count);
}

void memmgr_scan_mods()
{
    module_t* mods = mboot_get()->mods_addr;
    uintptr_t end  = _fblk->addr + _fblk->sz;
    size_t    sz   = 0;

    for (size_t i = 0; i < mboot_get()->mods_count; i++) 
    { 
        sz += mods[i].addr_end - mods[i].addr_start;
        memmgr_map(mods[i].addr_start, mods[i].addr_end - mods[i].addr_start, MEM_MODULE);
        end = mods[i].addr_end;
    }
    _fblk->addr = end;
    _fblk->sz -= sz;

    debug_log("%s Finished scanning modules - %u found\n", DEBUG_INFO, mboot_get()->mods_count);
}

void memmgr_init()
{
    memset(_memblks, 0, sizeof(_memblks));
    _count = 0;

    memmgr_scan_mmap();
    memmgr_scan_mods();

    debug_log("%s Zero-filling unused memory", DEBUG_INFO);
    for (size_t i = 0; i < MEMBLK_COUNT; i++)
    {
        if (_memblks[i].type == MEM_FREE) 
        { 
            size_t addr = _memblks[i].addr, sz = _memblks[i].sz;
            for (int i = 0; i < 8; i++)
            {
                memset((void*)addr, 0, _memblks[i].sz / 8);
                addr += _memblks[i].sz / 8;
                sz -= _memblks[i].sz / 8;
                debug_log(".");
            }
        }
    }

    debug_log("\n%s Initialized physical memory manager\n", DEBUG_OK);
    debug_log("%s Physical memory usage: %u/%u KB\n", DEBUG_INFO, memmgr_amount_used(KILOBYTE), memmgr_amount_installed(KILOBYTE));
}

void memmgr_print()
{
    /*
    debug_log("%s Displaying list of all memblks:\n", DEBUG_INFO);
    debug_log("|--------------------------------------------|\n");
    debug_log("| ADDR              TYPE        SIZE         |\n");
    debug_log("|--------------------------------------------|\n");

    char line[47];

    for (size_t i = 0; i < MEMBLK_COUNT; i++)
    {
        if (_memblks[i].type == MEM_INVALID) { continue; }

        memset(line, 0, sizeof(line));
        sprintf(line, "| %8x-%8x %s %a", _memblks[i].addr, _memblks[i].addr + _memblks[i].sz, memmgr_typestr(_memblks[i].type), _memblks[i].sz);
        while (strlen(line) < 45) 
        {
            char sp = 0x20;
            strncat(line, &sp, 1); 
        }
        debug_log("%s|\n", line);
    }
    debug_log("|--------------------------------------------|\n");
    */
}

memblk_t* memmgr_map(uintptr_t addr, size_t sz, MEMTYPE type)
{
    if (sz == 0)             { debug_error("memmgr_map(%8x, %a, %s) - Invalid size", addr, sz, memmgr_typestr(type)); return NULL; }
    if (type == MEM_INVALID) { debug_error("memmgr_map(%8x, %a, %s) - Invalid type", addr, sz, memmgr_typestr(type)); return NULL; }

    for (size_t i = 0; i < MEMBLK_COUNT; i++)
    {
        if (_memblks[i].type == MEM_INVALID)
        {
            _memblks[i] = (memblk_t){ addr, sz, type };
            _count++;
            debug_log("%s ADDR:%8x-%8x TYPE:%s SIZE:%a\n", DEBUG_MMAP, addr, addr + sz, memmgr_typestr(type), sz);
            return &_memblks[i];
        }
    }

    debug_error("memmgr_map(%8x, %a, %s) - Failed to map memblk", addr, sz, memmgr_typestr(type));
    return NULL;
}

void memmgr_unmap(memblk_t* memblk)
{
    if (memblk == NULL) { debug_error("memmgr_unmap(%8x) - Null pointer", memblk); return; }

    for (size_t i = 0; i < MEMBLK_COUNT; i++)
    {
        if (memcmp(&_memblks[i], memblk, sizeof(memblk_t)) == 0)
        {
            _count--;
            debug_log("%s ADDR:%8x-%8x TYPE:%s SIZE:%a\n", DEBUG_MUNMAP, _memblks[i].addr, _memblks[i].addr + _memblks[i].sz, memmgr_typestr(_memblks[i].type), _memblks[i].sz);
            _memblks[i] = (memblk_t){ 0, 0, MEM_INVALID };
            return;
        }
    }
    debug_error("memmgr_unmap(%8x) - Failed to unmap memblk", memblk);
}

memblk_t* memmgr_alloc(size_t sz, MEMTYPE type)
{
    if (sz == 0) { debug_error("memmgr_alloc(%a) - Invalid size", sz); return NULL; }
    if (type == MEM_INVALID || type == MEM_FREE) { debug_error("memmgr_alloc(%a) - Invalid type", sz); return NULL; }

    memblk_t* blk = NULL;

    size_t i;
    for (i = 0; i < MEMBLK_COUNT; i++)
    {
        if (_memblks[i].type == MEM_FREE && _memblks[i].sz >= sz)
        {
            if (_memblks[i].sz > sz)
            {
                blk = memmgr_map(_memblks[i].addr, sz, type);

                _memblks[i].addr += sz;
                _memblks[i].sz   -= sz;
                debug_log("%s ADDR:%8x-%8x TYPE:%s SIZE:%a\n", DEBUG_MCHG, _memblks[i].addr, _memblks[i].addr + _memblks[i].sz, memmgr_typestr(_memblks[i].type), _memblks[i].sz);
            }
            else
            {
                _memblks[i].type = type;
                blk = &_memblks[i];
                debug_log("%s ADDR:%8x-%8x TYPE:%s SIZE:%a\n", DEBUG_MCHG, _memblks[i].addr, _memblks[i].addr + _memblks[i].sz, memmgr_typestr(_memblks[i].type), _memblks[i].sz);
            }
            break;
        }
    }

    if (blk == NULL) { debug_error("memmgr_alloc(%a) - Failed to allocate physical memblk", sz); return NULL; }

    debug_log("%s ADDR:%8x-%8x TYPE:%s SIZE:%a\n", DEBUG_MREQ, blk->addr, blk->addr + blk->sz, memmgr_typestr(blk->type),blk->sz);
    return blk;
}

memblk_t* memmgr_from_addr(uintptr_t addr)
{
    for (size_t i = 0; i < MEMBLK_COUNT; i++)
    {
        if (_memblks[i].addr == addr && _memblks[i].type != MEM_INVALID) { return &_memblks[i]; }
    }
    return NULL;
}

memblk_t* memmgr_from_index(int index)
{
    if (index < 0 || index >= MEMBLK_COUNT) { return NULL; }
    return &_memblks[index];
}

memblk_t* memmgr_first_of(MEMTYPE type, size_t minsz)
{
    for (size_t i = 0; i < MEMBLK_COUNT; i++)
    {
        if (_memblks[i].type == type && _memblks[i].sz >= minsz) { return &_memblks[i]; }
    }
    return NULL;
}

uint32_t memmgr_amount_installed(int measurement)
{
    register uint64_t* mem;
    uint32_t amount = 0;
    uint64_t mem_count, a;
    uint8_t	irq1, irq2;

    irq1 = inb(0x21);
    irq2 = inb(0xA1);
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
    mem_count = 0;
    amount = 0;
    inline_asm("wbinvd");

    do 
    {
        amount++;
        mem_count += 1024 * 1024;
        mem = (uint64_t*)mem_count;

        a = *mem;
        *mem = 0x55AA55AA;

        inline_asm("":::"memory");
        if (*mem != 0x55AA55AA) { mem_count = 0; }
        else 
        {
            *mem = 0xAA55AA55;
            inline_asm("":::"memory");
            if (*mem != 0xAA55AA55) { mem_count = 0; }
        }

        inline_asm("":::"memory");
        *mem = a;

    } while (amount < 4096 && mem_count != 0);

    mem = (uint64_t*)0x413;
    outb(0x21, irq1);
    outb(0xA1, irq2);

    if (measurement == 0)             { amount *= MEGABYTE; }
    else if (measurement == KILOBYTE) { amount *= KILOBYTE; }
    else if (measurement == GIGABYTE) { amount /= GIGABYTE; }
    return amount;
}

uint32_t memmgr_amount_used(int measurement)
{
    uint32_t amount = 0;

    for (size_t i = 0; i < MEMBLK_COUNT; i++)
    {
        if (_memblks[i].type != MEM_INVALID && _memblks[i].type != MEM_FREE) { amount += _memblks[i].sz; }
    }

    if (measurement == KILOBYTE)      { amount /= KILOBYTE; }
    else if (measurement == MEGABYTE) { amount /= MEGABYTE; }
    else if (measurement == GIGABYTE) { amount /= GIGABYTE; }
    return amount;
}

uint32_t memmgr_amount_free(int measurement)
{
    return memmgr_amount_installed(measurement) - memmgr_amount_used(measurement);
}

const char* memmgr_typestr(MEMTYPE type)
{
    static const char* names[] = 
    {
        "invalid     ",
        "free        ",
        "reserved    ",
        "acpi_reclaim",
        "nvs         ",
        "bad_ram     ",
        "video_ram   ",
        "kernel      ",
        "module      ",
        "pgfrm_alloc ",
        "heap        ",
        "allocation  ",
        "unused      ",
        "vm_read     ",
        "vm_readwrite",
    };

    return names[type >= MEM_COUNT ? MEM_INVALID : type]; 
}