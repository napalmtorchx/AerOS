#pragma once
#include <lib/types.h>
#include <lib/ctype.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/kresult.h>
#include <lib/math.h>
#include <lib/ansi.h>
#include <lib/numerics/point.h>
#include <lib/numerics/rect.h>
#include <core/multiboot.h>
#include <core/memory/memmgr.h>
#include <core/memory/heap.h>
#include <core/fs/ramfs.h>
#include <core/fs/virtfs.h>
#include <debug/debug.h>
#include <debug/tests.h>
#include <graphics/color.h>
#include <graphics/font.h>
#include <graphics/image.h>
#include <graphics/console.h>
#include <hal/uart.h>
#include <hal/ports.h>
#include <hal/device.h>
#include <hal/devmgr.h>
#include <hal/ints/gdt.h>
#include <hal/ints/idt.h>
#include <hal/ints/pic.h>
#include <hal/ints/realmode.h>
#include <hal/devices/test_device.h>
#include <hal/devices/vga.h>
#include <hal/devices/vbe.h>

void kernel_boot();
void kernel_loop();

uintptr_t kernel_addr_start();
uintptr_t kernel_addr_end();
size_t    kernel_size();

uintptr_t kernel_stack_start();
uintptr_t kernel_stack_end();
size_t    kernel_stack_size();

heap_t* kernel_heap_ref();

multiboot_t* mboot_get();
ramfs_t* bootfs_get();
font_t* sysfont_get();