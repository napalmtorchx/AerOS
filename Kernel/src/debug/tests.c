#include <debug/tests.h>
#include <kernel.h>

void test_all()
{
    test_memcpyq();
    test_runtime();
    test_graphics();
}

void test_runtime()
{
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

void test_memcpyq()
{
    uint32_t test_src[4] = 
    {
        0xDEADBEEF,
        0xBADC0DE1,
        0x55AAAA55,
        0xFFFFFFFF
    };
    uint32_t test_dst[4] = { 0 };

    memcpyq(test_dst, test_src, 2);
    for (int i = 0; i < 4; i++) { debug_log(DEBUG_INFO " %8x\n", test_dst[i]); }
}

void test_graphics()
{
    transparent_rect(0, 0, 100, 100, COLOR_DARKCYAN);
}