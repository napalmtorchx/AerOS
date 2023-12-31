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
#include <core/threading/thread.h>
#include <core/threading/taskmgr.h>
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
#include <hal/cpu.h>
#include <hal/fpu.h>
#include <hal/registers.h>
#include <hal/ints/gdt.h>
#include <hal/ints/idt.h>
#include <hal/ints/pic.h>
#include <hal/ints/realmode.h>
#include <hal/devices/video/vga.h>
#include <hal/devices/video/vbe.h>
#include <hal/devices/input/ps2.h>
#include <hal/devices/input/kbd.h>
#include <hal/devices/sys/pit.h>
#include <hal/devices/sys/rtc.h>
#include <hal/devices/sys/pci.h>
#include <hal/devices/sys/cpu_opt.h>
#include <services/cmdhost.h>
#include <services/gfxhost.h>
#include <runtime/executable.h>
#include <runtime/runtime.h>

void kernel_boot();
void kernel_init_graphics();
void kernel_loop();

uintptr_t kernel_addr_start();
uintptr_t kernel_addr_end();
size_t    kernel_size();

uintptr_t kernel_stack_start();
uintptr_t kernel_stack_end();
size_t    kernel_stack_size();

bool kernel_booted();

heap_t* kernel_heap_ref();

multiboot_t* mboot_get();
ramfs_t* bootfs_get();
font_t* sysfont_get();
console_t* kconsole_get();