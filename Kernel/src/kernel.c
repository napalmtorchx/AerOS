#include <kernel.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _stack_top;
extern uint32_t _stack_bottom;

static multiboot_t* _multiboot;
static ramfs_t      _bootfs;
static heap_t       _kernel_heap;
static font_t*      _sysfont;
static console_t    _console;
//add this is a char array so we can change its value later on with a different font
char* font = "A:/unifont.sfn";
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

    gdt_init();
    idt_init();
    memmgr_init();
    _kernel_heap = init_kernel_heap(false);
    devmgr_init();
    virtfs_init();    

    fpu_init();
    vbe_setmode(1024, 768);
    // attempt to load system font
    FILE* file = fopen(font, "r");
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
}

void kernel_loop()
{
    debug_log("%s Entered kernel main\n", DEBUG_INFO);

    time_t t;
    int sec, fps, frames;
    console_printf(kconsole_get(), "AerOS version 2.0\nRAM:%u/%u KB\n", heap_get_used_mem(&_kernel_heap) / KILOBYTE, heap_get_total_mem(&_kernel_heap) / KILOBYTE);

    pci_init();
    console_printf(kconsole_get(), "RAM after PCI:%u/%u KB\n", heap_get_used_mem(&_kernel_heap) / KILOBYTE, heap_get_total_mem(&_kernel_heap) / KILOBYTE);

   /*char*       buff  = (char*)malloc(1024);
    console_printf(kconsole_get(), vbe_available_resolutions(buff));
    free(buff);*/
    while (true)
    {
        frames++;
        t = time(NULL);
        if (sec != t.second)
        {
            sec = t.second;
            fps = frames;
            frames = 0;

            char buff[64];
            memset(buff, 0, sizeof(buff));
       //     console_printf(kconsole_get(), "FPS:%d Time:%s\n", fps, timestr(&t, buff, TIMEFORMAT_STANDARD, true));
        }
        
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