#include <kernel.h>
#define DEFAULT_FONT_PATH "A:/unifont.sfn"

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _stack_top;
extern uint32_t _stack_bottom;

static multiboot_t* _multiboot;
static ramfs_t      _bootfs;
static heap_t       _kernel_heap;
static font_t*      _sysfont;
static console_t    _console;

void kernel_main(multiboot_t* mboot)
{
    _multiboot = mboot;
    
    kernel_boot();
    kernel_loop();
}

void kernel_boot()
{
    debug_init();
    debug_print("Starting AerOS v2.0...\nBootloader:");
    debug_println(_multiboot->bootloader_name);
sse_enable();
    gdt_init();
    idt_init();
    memmgr_init();
    _kernel_heap = init_kernel_heap(false);
    
    
    fpu_init();
    devmgr_init();
    vbe_setmode(1024, 768);
    virtfs_init();    
    taskmgr_init();
    cmdhost_init();

    // attempt to load system font
    FILE* file = fopen(DEFAULT_FONT_PATH, "r");
    if (file == NULL) { debug_log("%s Failed to locate file 'A:/unifont.sfn'\n", DEBUG_ERROR); }
    else
    {
        ssfn_font_t* filedata = (ssfn_font_t*)malloc(file->sz);
        fread(filedata, file->sz, 1, file);
        fclose(file);
        _sysfont = font_create_ssfn(filedata, 0, 0);
    }

    // initialize console
    vbe_device_t* vbe = devmgr_from_name("vbe_controller");
    _console = console_create((image_t){ vbe->w, vbe->h, vbe->fbptr }, _sysfont, COLOR_WHITE, COLOR_BLACK, 64 * KILOBYTE);

    static const uint8_t prog[sizeof(executable_t) + 17] = 
    {
        // name - 'Test Program'
        0x22, 0x54, 0x65, 0x73, 0x74, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // type - EXEC_SERVICE
        0x01, 0x00, 0x00, 0x00,
        // text offset
        0x00, 0x00, 0x00, 0x00,
        // text size
        17, 0x00, 0x00, 0x00,
        // data offset
        0x00, 0x00, 0x00, 0x00,
        // data size
        0x00, 0x00, 0x00, 0x00,

        // --------- CODE ---------------

        // ld R4, 0x1000 - load size to malloc
        (uint8_t)RTOP_LD, (uint8_t)RTREG_R4, 0x00, 0x10, 0x00, 0x00,
        // malloc(R4);
        (uint8_t)RTOP_SYSCALL, 0x01, 0x00, 0x00, 0x00,
        // free(R5);
        (uint8_t)RTOP_SYSCALL, 0x02, 0x00, 0x00, 0x00,
        (uint8_t)RTOP_EXIT,
    };
    
    // runtime test
    runtime_t runtime = runtime_create((uint8_t*)prog, sizeof(prog), 0x10000);
    runtime_run(&runtime);   

    uint32_t test_src[4] = {
        0xDEADBEEF,
        0xBADC0DE1,
        0x55AAAA55,
        0xFFFFFFFF
    };
    uint32_t test_dst[4] = { 0 };

    memcpyq(test_dst, test_src, 2);
    for (int i = 0; i < 4; i++) { debug_log(DEBUG_INFO " %8x\n", test_dst[i]); }
}

void kernel_loop()
{
    debug_log("%s Entered kernel main\n", DEBUG_INFO);
    taskmgr_toggle(true);

    time_t t;
    int sec, fps, frames;
    console_printf(kconsole_get(), "AerOS version 2.0\nRAM:%u/%u KB\n", heap_get_used_mem(&_kernel_heap) / KILOBYTE, heap_get_total_mem(&_kernel_heap) / KILOBYTE);
    get_cpu_name();
    sse_enable(); //this sets the cpu to use sse instructions
    enable_optimized_sse(); //this enables the optimized sse instructions
     
    pci_init();

    if(is_qemu()) debug_log("Running in qemu\n");
    else debug_log("Running on real hardware or other VM\n");
    console_printf(kconsole_get(), "RAM after PCI:%u/%u KB\n", heap_get_used_mem(&_kernel_heap) / KILOBYTE, heap_get_total_mem(&_kernel_heap) / KILOBYTE);

    while (true)
    {
        lock();

        frames++;
        t = time(NULL);
        if (sec != t.second)
        {
            sec = t.second;
            fps = frames;
            frames = 0;
        }
        debug_log("K");
        taskmgr_schedule(true);
    }   
}

uintptr_t kernel_addr_start() { return (uint32_t)&_kernel_start; }

uintptr_t kernel_addr_end() { return (uint32_t)&_kernel_end; }

size_t kernel_size() { return kernel_addr_end() - kernel_addr_start(); }

uintptr_t kernel_stack_start() { return (uint32_t)&_stack_bottom; }

uintptr_t kernel_stack_end() { return (uint32_t)&_stack_top; }

size_t kernel_stack_size() { return kernel_stack_end() - kernel_stack_start(); }

multiboot_t* mboot_get() { return _multiboot; }

ramfs_t* bootfs_get(void) { return &_bootfs; }

font_t* sysfont_get() { return _sysfont; }

heap_t* kernel_heap_ref() { return &_kernel_heap; }

console_t* kconsole_get() 
{ 
    if (_console.size.x == 0 || _console.size.y == 0 || _console.font == NULL || _console.img.buffer == NULL) { return NULL; }
    return &_console; 
}