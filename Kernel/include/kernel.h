#pragma once
#include <lib/types.h>
#include <lib/ctype.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/kresult.h>
#include <lib/math.h>
#include <core/multiboot.h>
#include <core/memory/memmgr.h>
#include <debug/debug.h>
#include <debug/tests.h>
#include <hal/uart.h>
#include <hal/ports.h>
#include <hal/device.h>
#include <hal/devmgr.h>
#include <hal/devices/test_device.h>
#include <hal/devices/vga.h>

void kernel_boot();
void kernel_loop();

uintptr_t kernel_addr_start();
uintptr_t kernel_addr_end();
size_t    kernel_size();

uintptr_t kernel_stack_start();
uintptr_t kernel_stack_end();
size_t    kernel_stack_size();

multiboot_t* mboot_get();