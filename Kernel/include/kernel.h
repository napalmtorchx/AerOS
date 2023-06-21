#pragma once
#include <lib/types.h>
#include <lib/ctype.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <core/multiboot.h>

namespace aeros
{
    class Kernel
    {
        public:
            static multiboot_t* multiboot;

        public:
            static void boot();
            static void main();
    };
}